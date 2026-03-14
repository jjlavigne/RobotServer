#include "RemoteWebcamProviderWorker.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>


RemoteWebcamProviderWorker::RemoteWebcamProviderWorker(std::shared_ptr<SensorDataDispatcher> dispatcher) 
    : dispatcher_(dispatcher), sockfd_(-1), running_(false) {}

RemoteWebcamProviderWorker::~RemoteWebcamProviderWorker() {
    stop();
}

void RemoteWebcamProviderWorker::start() {
    std::cout << "RWP start" << std::endl;
    running_ = true;

    sockfd_ = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd_ < 0) {
        perror("Socket creation failed");
        return;
    }

    // Set a timeout so recvfrom doesn't block forever, allowing clean shutdown
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 100000; // 100ms
    setsockopt(sockfd_, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    struct sockaddr_in servaddr;
    std::memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    servaddr.sin_port = htons(5005);

    if (bind(sockfd_, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Bind failed");
        close(sockfd_);
        return;
    }

    std::cout << "RemoteWebcamProviderWorker started on port 5005..." << std::endl;

    receiveLoop();
}

void RemoteWebcamProviderWorker::stop() {
    if (!running_) return;
    running_ = false;

    if (worker_thread_.joinable()) {
        worker_thread_.join();
    }
    if (sockfd_ != -1) {
        close(sockfd_);
        sockfd_ = -1;
    }
    std::cout << "RemoteWebcamProviderWorker stopped." << std::endl;
}

void RemoteWebcamProviderWorker::receiveLoop() {
    std::cout << "recieve loop entered" << std::endl;
    char buffer[1024];
    while (running_) {
        ssize_t n = recvfrom(sockfd_, buffer, sizeof(buffer) - 1, 0, nullptr, nullptr);
        if (n == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                std::cout << "didnt get data: " << errno <<std::endl;
                // Didn't get data, retrying...
            } else {
                std::cout << "Error reading socket error: " << errno <<std::endl;
                return;
            }
        }
        if (n > 0) {
            buffer[n] = '\0'; // Make sure it's a valid string
            std::cout << "[Receiver] Got: " << buffer << std::endl;

            // 1. Create a fresh, empty SensorData packet
            auto data = std::make_shared<SensorData>();
            ShapeData incomingShape;

            // 2. Parse the string! 
            // We expect the Python script to send: "SHAPE,x,y,width,height,r,g,b"
            // sscanf will extract those numbers and drop them directly into our struct variables.
            int parsedItems = sscanf(buffer, "SHAPE,%d,%d,%d,%d,%d,%d,%d", 
                                     &incomingShape.x, &incomingShape.y, 
                                     &incomingShape.width, &incomingShape.height,
                                     &incomingShape.r, &incomingShape.g, &incomingShape.b);

            // 3. Did we successfully grab all 7 numbers?
            if (parsedItems == 7) {
                // Yes! Populate the struct inside our packet
                data->shape = incomingShape; 
                
                // 4. Hand the populated packet to the dispatcher
                dispatcher_->enqueueData(data); 
            }
        }
    }
}