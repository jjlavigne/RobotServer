#pragma once 

#include "WorkerInterface.h"
#include "SensorDataDispatcherInterface.h"
#include <vector>
#include <memory>
#include <thread>

class SensorDataManager {
    public:
    SensorDataManager(std::shared_ptr<SensorDataDispatcherInterface> dispatcher, std::vector<std::shared_ptr<WorkerInterface>> providers);
    ~SensorDataManager() = default;
    void start();
    void stop();

    private:

    std::shared_ptr<SensorDataDispatcherInterface> dispatcher_;
    std::vector<std::shared_ptr<WorkerInterface>> providers_;
    std::vector<std::thread> workerThreads_;
};