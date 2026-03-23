#include "RobotControlWorker.h"
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

const char* ARDUINO_IP = "192.168.4.25";
const int ARDUINO_PORT = 4210;

RobotControlWorker::RobotControlWorker()
    : queue_(folly::ProducerConsumerQueue<std::shared_ptr<SensorData>>(100)) {}

void RobotControlWorker::enqueue(std::shared_ptr<SensorData> data) {
    if (!data->userInput.has_value()) {
        return;
    }
    queue_.write(data);
}

void RobotControlWorker::start() {
    arduinoSocket_ = socket(AF_INET, SOCK_DGRAM, 0);
    if (arduinoSocket_ < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return;
    }
    arduinoAddr_.sin_family = AF_INET;
    arduinoAddr_.sin_port = htons(ARDUINO_PORT);
    inet_pton(AF_INET, ARDUINO_IP, &arduinoAddr_.sin_addr);

    isRunning_ = true;
    while (isRunning_) {
        // std::cout << "RCW while IsRunning Entered" << std::endl;
        if (!queue_.isEmpty()) {
            std::shared_ptr<SensorData> sensorData;
            if (queue_.read(sensorData)) {
                process(sensorData);
            } else {
            }
        }
    }

    // Cleanup
    close(arduinoSocket_);
}

void RobotControlWorker::stop() { isRunning_ = false; }

void RobotControlWorker::process(std::shared_ptr<SensorData> data) {
    if (data->userInput.has_value()) {
        processUserInput(data);
    }

    // if (data->LLMInput.has_value()) {
    //     processLLMInput();
    // }
}

void RobotControlWorker::processUserInput(std::shared_ptr<SensorData> data) {

    bool forwardKeyPressed = data->userInput.value().forward;
    bool backwardKeyPressed = data->userInput.value().backward;
    bool leftKeyPressed = data->userInput.value().left;
    bool rightKeyPressed = data->userInput.value().right;

    // Note: Your comment says "Only send packet if the state CHANGED"
    // but this logic still fires every single frame the key is held!
    if (forwardKeyPressed) {
        sendUDP("MOTOR, FORWARD");
    } else if (backwardKeyPressed) {
        sendUDP("MOTOR, BACKWARD");
    } else if (leftKeyPressed) {
        sendUDP("MOTOR, LEFT");
    } else if (rightKeyPressed) {
        sendUDP("MOTOR, RIGHT");
    } else {
        sendUDP("MOTOR, STOP");
    }
}

void RobotControlWorker::processLLMInput(std::shared_ptr<SensorData> data) {}

void RobotControlWorker::sendUDP(const char* message) {
    sendto(arduinoSocket_, message, strlen(message), 0,
           (struct sockaddr*)&arduinoAddr_, sizeof(arduinoAddr_));
}
