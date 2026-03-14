#pragma once

#include "SensorDataDispatcherInterface.h"
#include "SensorDataWorkerInterface.h"
#include <vector>
#include <memory>

class SensorDataDispatcher : public SensorDataDispatcherInterface{
    public: 
    SensorDataDispatcher(std::vector<std::shared_ptr<SensorDataWorkerInterface>> processors);

    void enqueueData(std::shared_ptr<SensorData> data) override;
    void addProcessor(std::shared_ptr<SensorDataWorkerInterface> processor);

    private:
    
    std::vector<std::shared_ptr<SensorDataWorkerInterface>> processors_;
};
