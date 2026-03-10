#include "SimulatedWebcamSender.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <chrono>

SimulatedWebcamSender::SimulatedWebcamSender() : sockfd_(-1), running_(false) {}

SimulatedWebcamSender::~SimulatedWebcamSender() {
    stop();
}

void SimulatedWebcamSender::start() {
    if (running_) return;

    sockfd_ = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd_ < 0) {
        perror("Simulator socket failed");
        return;
    }

    running_ = true;
    worker_thread_ = std::thread(&SimulatedWebcamSender::sendLoop, this);
    std::cout << "Simulator started..." << std::endl;
}

void SimulatedWebcamSender::stop() {
    if (!running_) return;
    running_ = false;

    if (worker_thread_.joinable()) {
        worker_thread_.join();
    }
    if (sockfd_ != -1) {
        close(sockfd_);
        sockfd_ = -1;
    }
    std::cout << "Simulator stopped." << std::endl;
}

void SimulatedWebcamSender::sendLoop() {
    struct sockaddr_in dest_addr;
    std::memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(9999);
    inet_pton(AF_INET, "127.0.0.1", &dest_addr.sin_addr);

    int frame_count = 0;
    while (running_) {
        // Simulating sending a video frame or sensor data
        std::string payload = "Simulated Frame Data #" + std::to_string(frame_count++);
        
        sendto(sockfd_, payload.c_str(), payload.length(), 0, 
               (const struct sockaddr *)&dest_addr, sizeof(dest_addr));
        
        // Sleep for ~33ms to simulate 30 FPS
        std::this_thread::sleep_for(std::chrono::milliseconds(33));
    }
}