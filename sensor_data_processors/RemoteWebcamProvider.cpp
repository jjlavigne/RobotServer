#include "RemoteWebcamProvider.h"
#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
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
        // Clear the buffer with zeros before every receive
        std::memset(buffer, 0, sizeof(buffer));

        struct sockaddr_in clientaddr;
        socklen_t len = sizeof(clientaddr);
        
        ssize_t n = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&clientaddr, &len);
        
        if (n > 0) {
            char senderIP[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(clientaddr.sin_addr), senderIP, INET_ADDRSTRLEN);
            
            // Only process if it's coming from the Pi (e.g., 192.168.4.X)
            if (strstr(senderIP, "192.168.4.") != NULL) {
                std::cout << "Valid message from PI (" << senderIP << "): " << buffer << std::endl;
            } else {
                std::cout << "Ignoring junk from: " << senderIP << std::endl;
            }
        }
    }

    close(sockfd);
}

void RemoteWebcamProvider::stop() {
    
}