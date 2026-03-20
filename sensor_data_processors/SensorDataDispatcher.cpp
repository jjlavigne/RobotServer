#include <iostream>
#include "SensorDataDispatcher.h"
#include <thread>

SensorDataDispatcher::SensorDataDispatcher(std::vector<std::shared_ptr<SensorDataWorkerInterface>> processors) : processors_(std::move(processors)) {}

void SensorDataDispatcher::enqueueData(std::shared_ptr<SensorData> data) {
    for (auto& processor : processors_) {
        processor->enqueue(data); 
    }
}

void SensorDataDispatcher::addProcessor(std::shared_ptr<SensorDataWorkerInterface> processor) {
    if (processor != nullptr) {
        processors_.push_back(processor);
    }
}
