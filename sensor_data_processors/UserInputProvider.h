#pragma once

#include "SensorDataProviderInterface.h"
#include "SensorDataDispatcherInterface.h"

class UserInputProvider : public SensorDataProviderInterface{
public: 
    UserInputProvider(std::shared_ptr<SensorDataDispatcherInterface> provider) : provider_(provider) {};
    virtual ~UserInputProvider() override = default;
    void start() override;
    void stop() override;
private:
    std::shared_ptr<SensorDataDispatcherInterface> provider_;
};