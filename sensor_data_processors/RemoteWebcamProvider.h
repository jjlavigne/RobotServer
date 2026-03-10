#pragma once
#include "SensorDataProviderInterface.h"
#include "SensorDataDispatcher.h" // <-- This fixes the "not declared" error
#include <thread>
#include <atomic>
#include <memory>

class RemoteWebcamProvider : public SensorDataProviderInterface {
public:
    // <-- This fixes the constructor mismatch error
    RemoteWebcamProvider(std::shared_ptr<SensorDataDispatcher> dispatcher);
    ~RemoteWebcamProvider() override;

    void start() override;
    void stop() override;

private:
    void receiveLoop();

    std::shared_ptr<SensorDataDispatcher> dispatcher_;
    int sockfd_;
    std::atomic<bool> running_;
    std::thread worker_thread_;
};