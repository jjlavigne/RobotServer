#include "RobotControlWorker.h"
#include <arpa/inet.h>
#include <chrono>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

const char* ARDUINO_IP = "192.168.4.40";
const int ARDUINO_PORT = 4210;

RobotControlWorker::RobotControlWorker()
    : queue_(folly::ProducerConsumerQueue<std::shared_ptr<SensorData>>(100)) {}

void RobotControlWorker::enqueue(std::shared_ptr<SensorData> data) {
    if (!data->userInput.has_value() && !data->llmInput.has_value()) {
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
            std::cout << "queue not empty" << std::endl;
            std::shared_ptr<SensorData> sensorData;
            if (queue_.read(sensorData)) {
                std::cout << "queue read" << std::endl;
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
    // hijack user input to convert to llm input
    // if w pressed down, then convert that to sensordata for forward 1 ft
    // if D pressed down,

    std::cout << "RCW process" << std::endl;

    if (data->userInput.has_value()) {
        processUserInput(data);
    }

    if (data->llmInput.has_value()) {
        std::cout << "hasvalue" << std::endl;
        processLLMInput(data);
    }
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
        std::cout << "Motor forward sent" << std::endl;
    } else if (backwardKeyPressed) {
        sendUDP("MOTOR, BACKWARD");
        std::cout << "Motor backward sent" << std::endl;
    } else if (leftKeyPressed) {
        sendUDP("MOTOR, LEFT");
        std::cout << "Motor left sent" << std::endl;
    } else if (rightKeyPressed) {
        sendUDP("MOTOR, RIGHT");
        std::cout << "Motor right sent" << std::endl;
    } else {
        sendUDP("MOTOR, STOP");
        std::cout << "Motor stop sent" << std::endl;
    }
}

void RobotControlWorker::processLLMInput(std::shared_ptr<SensorData> data) {
    // read data, find out which movement to take
    // loop for amount of time to do that movement
    std::cout << "LLM process called" << std::endl;

    int timeToGoHalfFootMs = 940;
    int timeToGo45DegreesMs = 600;
    int sleepTime = 0;

    if (data->llmInput.has_value()) {

        std::cout << "LLMInput has value" << std::endl;

        if (data->llmInput->type == MovementType::Forward) {
            sleepTime = ((data->llmInput->value) / 0.5) * timeToGoHalfFootMs;
            sendUDP("MOTOR, FORWARD");
            // std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            sendUDP("MOTOR, STOP");
        } else if (data->llmInput->type == MovementType::Backward) {
            sleepTime = ((data->llmInput->value) / 0.5) * timeToGoHalfFootMs;
            sendUDP("MOTOR, BACKWARD");
            // std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            sendUDP("MOTOR, STOP");
        } else if (data->llmInput->type == MovementType::Left) {
            sendUDP("MOTOR, LEFT");
            sleepTime = ((data->llmInput->value) / 45) * timeToGo45DegreesMs;
            // std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            sendUDP("MOTOR, STOP");
        } else if (data->llmInput->type == MovementType::Right) {
            sleepTime = ((data->llmInput->value) / 45) * timeToGo45DegreesMs;
            sendUDP("MOTOR, RIGHT");
            // std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            sendUDP("MOTOR, STOP");
        } else if (data->llmInput->type == MovementType::Stop) {
            sendUDP("MOTOR, STOP");
        }
    }
}
void RobotControlWorker::sendUDP(const char* message) {
    std::cout << "sendUdp: " << message << std::endl;
    sendto(arduinoSocket_, message, strlen(message), 0,
           (struct sockaddr*)&arduinoAddr_, sizeof(arduinoAddr_));
}
