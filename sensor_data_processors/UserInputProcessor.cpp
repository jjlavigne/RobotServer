#include <iostream>
#include "UserInputProcessor.h"

void UserInputProcessor::process(std::shared_ptr<SensorData> data) {
    std::cout << "backward: " << data->userInput.value().backward << " forward: " << data->userInput.value().forward << " left: " << data->userInput.value().left << " right: " << data->userInput.value().right << std::endl;
    std::cout << "UserInputProcessor called" << std::endl;
}