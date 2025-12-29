#include <iostream>
#include "SensorDataDispatcher.h"

void SensorDataDispatcher::enqueueData(std::shared_ptr<SensorData> data) {
    for (auto& processor : processors_) {
        processor->process(data);
    }
}

