#pragma once

#include <atomic>
#include <mutex>   // ADDED: For thread safety
#include <memory>

#include "WorkerInterface.h"
#include "SensorDataDispatcherInterface.h"
#include "SensorDataWorkerInterface.h"
#include <folly/ProducerConsumerQueue.h>

class SDLWorker : public SensorDataWorkerInterface {
public: 
    SDLWorker(std::shared_ptr<SensorDataDispatcherInterface> dispatcher);
    virtual ~SDLWorker() override = default;
    void start() override;
    void stop() override;
    void process(std::shared_ptr<SensorData> data) override;
    void enqueue(std::shared_ptr<SensorData> data) override;
private:
    std::shared_ptr<SensorDataDispatcherInterface> dispatcher_;
    std::atomic<bool> isRunning{false};

    std::mutex dataMutex_;
    std::shared_ptr<SensorData> latestData_ = nullptr;
    folly::ProducerConsumerQueue<std::shared_ptr<SensorData>> queue_;
    bool isRunning_ = true;
};