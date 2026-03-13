#pragma once
#include "WorkerInterface.h"
#include "SensorDataDispatcher.h"
#include <thread>
#include <atomic>
#include <memory>

class RemoteWebcamProviderWorker : public WorkerInterface {
public:
    RemoteWebcamProviderWorker(std::shared_ptr<SensorDataDispatcher> dispatcher);
    ~RemoteWebcamProviderWorker() override;

    void start() override;
    void stop() override;

private:
    void receiveLoop();

    std::shared_ptr<SensorDataDispatcher> dispatcher_;
    int sockfd_;
    std::atomic<bool> running_;
    std::thread worker_thread_;
};