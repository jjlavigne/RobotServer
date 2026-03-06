#pragma once

#include <atomic>

#include "SensorDataProviderInterface.h"
#include "SensorDataDispatcherInterface.h"

class RemoteWebcamProvider : public SensorDataProviderInterface{
public: 
    RemoteWebcamProvider(std::shared_ptr<SensorDataDispatcherInterface> dispatcher) : dispatcher_(std::move(dispatcher)) {}
    virtual ~RemoteWebcamProvider() override = default;
    void start() override;
    void stop() override;
private:
    std::shared_ptr<SensorDataDispatcherInterface> dispatcher_;
    std::atomic<bool> isRunning{false};
};