#include <iostream>
#include "UserInputProcessorWorker.h"

UserInputProcessorWorker::UserInputProcessorWorker(): queue_(folly::ProducerConsumerQueue<std::shared_ptr<SensorData>>(100)) {
    
}

void UserInputProcessorWorker::start() {
    while (isRunning_){
        if (!queue_.isEmpty()) {
            std::shared_ptr<SensorData> sensorData;
            if (queue_.read(sensorData)) {
                process(sensorData);
            }
            else {
                std::cout << "Queue failed to read" << std::endl;
            }
        }
    }
}

void UserInputProcessorWorker::stop() {
}

void UserInputProcessorWorker::process(std::shared_ptr<SensorData> data) {
    if (!data->userInput.has_value()) {
        return;
    }
     std::cout << "backward: " << data->userInput.value().backward << " forward: " << data->userInput.value().forward << " left: " << data->userInput.value().left << " right: " << data->userInput.value().right << std::endl;
    std::cout << "UserInputProcessorWorker called" << std::endl;
}

void UserInputProcessorWorker::enqueue(std::shared_ptr<SensorData> data) {
    if (!data->image.has_value()) {
        return;
    }
    queue_.write(data);
}
    
   