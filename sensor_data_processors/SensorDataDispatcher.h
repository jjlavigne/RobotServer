#pragma once

#include "SensorDataDispatcherInterface.h"
#include "SensorDataProcessorInterface.h"
#include <vector>
#include <memory>

class SensorDataDispatcher : public SensorDataDispatchInterface{
    public: 
    SensorDataDispatcher(std::vector<std::shared_ptr<SensorDataProcessorInterface>> processors);

    void enqueueData(std::shared_ptr<SensorData> data) override;

    private:
    
    std::vector<std::shared_ptr<SensorDataProcessorInterface>> processors_;
};
