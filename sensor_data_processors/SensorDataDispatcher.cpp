#include <iostream>
#include "SensorDataDispatcher.h"

SensorDataDispatcher::SensorDataDispatcher(std::vector<std::shared_ptr<SensorDataProcessorInterface>> processors) : processors_(std::move(processors)) {}

void SensorDataDispatcher::enqueueData(std::shared_ptr<SensorData> data) {
    for (auto& processor : processors_) {
        processor->process(data);
    }
}

