#pragma once

#include <atomic>
#include <mutex>   // ADDED: For thread safety
#include <memory>

#include "WorkerInterface.h"
#include "SensorDataDispatcherInterface.h"
#include "SensorDataWorkerInterface.h"

class SDLWorker : public SensorDataWorkerInterface {
public: 
    SDLWorker(std::shared_ptr<SensorDataDispatcherInterface> dispatcher) : dispatcher_(std::move(dispatcher)) {}
    virtual ~SDLWorker() override = default;
    void start() override;
    void stop() override;
    void process(std::shared_ptr<SensorData> data) override;
private:
    std::shared_ptr<SensorDataDispatcherInterface> dispatcher_;
    std::atomic<bool> isRunning{false};

    std::mutex dataMutex_;
    std::shared_ptr<SensorData> latestData_ = nullptr;
};