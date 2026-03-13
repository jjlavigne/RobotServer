#include <iostream>
#include "UserInputProcessorWorker.h"

void UserInputProcessorWorker::start() {
}

void UserInputProcessorWorker::stop() {
}

void UserInputProcessorWorker::process(std::shared_ptr<SensorData> data) {
    std::cout << "backward: " << data->userInput.value().backward << " forward: " << data->userInput.value().forward << " left: " << data->userInput.value().left << " right: " << data->userInput.value().right << std::endl;
    std::cout << "UserInputProcessorWorker called" << std::endl;
}