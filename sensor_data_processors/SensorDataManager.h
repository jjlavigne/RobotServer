#pragma once 

#include "SensorDataProviderInterface.h"
#include "SensorDataDispatcherInterface.h"
#include <vector>
#include <memory>

class SensorDataManager {
    public:
    SensorDataManager(std::shared_ptr<SensorDataDispatcherInterface> dispatcher, std::vector<std::shared_ptr<SensorDataProviderInterface>> providers);
    ~SensorDataManager() = default;
    void start();
    void stop();

    private:

    std::shared_ptr<SensorDataDispatcherInterface> dispatcher_;
    std::vector<std::shared_ptr<SensorDataProviderInterface>> providers_;

};