#pragma once 

#include "WorkerInterface.h"
#include "SensorDataDispatcherInterface.h"
#include <vector>
#include <memory>
#include <thread>

class WorkerManager {
    public:
    WorkerManager(std::shared_ptr<SensorDataDispatcherInterface> dispatcher, std::vector<std::shared_ptr<WorkerInterface>> workers, std::vector<std::shared_ptr<WorkerInterface>> asyncWorkers);
    ~WorkerManager() = default;
    void start();
    void stop();

    private:

    std::shared_ptr<SensorDataDispatcherInterface> dispatcher_;
    std::vector<std::shared_ptr<WorkerInterface>> workers_;
    std::vector<std::shared_ptr<WorkerInterface>> asyncWorkers_;
    std::vector<std::thread> workerThreads_;
};