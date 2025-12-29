#include <iostream>
#include "SensorDataDispatcher.h"

void SensorDataDispatcher::enqueueData(std::shared_ptr<SensorDataDispatch> data) {
    std::cout << "Wow!" << std::endl;
}