#include "RemoteWebcamProvider.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

RemoteWebcamProvider::RemoteWebcamProvider(std::shared_ptr<SensorDataDispatcher> dispatcher) 
    : dispatcher_(dispatcher), sockfd_(-1), running_(false) {}

RemoteWebcamProvider::~RemoteWebcamProvider() {
    stop();
}

void RemoteWebcamProvider::start() {
    if (running_) return;

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

    running_ = true;
    worker_thread_ = std::thread(&RemoteWebcamProvider::receiveLoop, this);
    std::cout << "RemoteWebcamProvider started on port 5005..." << std::endl;
}

void RemoteWebcamProvider::stop() {
    if (!running_) return;
    running_ = false;

    if (worker_thread_.joinable()) {
        worker_thread_.join();
    }
    if (sockfd_ != -1) {
        close(sockfd_);
        sockfd_ = -1;
    }
    std::cout << "RemoteWebcamProvider stopped." << std::endl;
}

void RemoteWebcamProvider::receiveLoop() {
    char buffer[1024];
    while (running_) {
        ssize_t n = recvfrom(sockfd_, buffer, sizeof(buffer) - 1, 0, nullptr, nullptr);
        if (n > 0) {
            buffer[n] = '\0';
            std::cout << "[Receiver] Got: " << buffer << std::endl;
        }
    }
}