#include "RemoteWebcamProvider.h"
#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <thread>
#include <SDL2/SDL.h>
#include "SensorDataProcessorInterface.h"

void RemoteWebcamProvider::start() {
    std::cout << "start entered" << std::endl;
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return;
    }

    struct sockaddr_in servaddr;
    std::memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    servaddr.sin_port = htons(9999); // Ensure this matches the Python port

    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        return;
    }

    std::cout << "Waiting for messages..." << std::endl;

    char buffer[1024];
    while (true) {
        // recvfrom blocks until a packet arrives
        ssize_t n = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, nullptr, nullptr);
        
        if (n > 0) {
            buffer[n] = '\0'; // Null-terminate the string
            std::cout << "Server received: " << buffer << std::endl;
        }
    }

    close(sockfd);
}

void RemoteWebcamProvider::stop() {
    
}