#pragma once

#include <atomic>

#include "WorkerInterface.h"
#include "SensorDataDispatcherInterface.h"

class SDLWorker : public WorkerInterface {
public: 
    SDLWorker(std::shared_ptr<SensorDataDispatcherInterface> dispatcher) : dispatcher_(std::move(dispatcher)) {}
    virtual ~SDLWorker() override = default;
    void start() override;
    void stop() override;
private:
    std::shared_ptr<SensorDataDispatcherInterface> dispatcher_;
    std::atomic<bool> isRunning{false};
};