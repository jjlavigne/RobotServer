#pragma once
#include "SensorDataProviderInterface.h"
#include <thread>
#include <atomic>
#include <string>

class SimulatedWebcamSender : public SensorDataProviderInterface {
public:
    SimulatedWebcamSender();
    ~SimulatedWebcamSender() override;

    void start() override;
    void stop() override;

private:
    void sendLoop();

    int sockfd_;
    std::atomic<bool> running_;
    std::thread worker_thread_;
};