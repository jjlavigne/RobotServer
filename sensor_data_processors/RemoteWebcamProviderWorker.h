#pragma once
#include "WorkerInterface.h"
#include "SensorDataDispatcher.h"
#include <atomic> 
#include <memory>
#include <vector>   
#include <map>      
#include <cstdint>

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
    std::map<uint32_t, std::map<uint16_t, std::vector<uint8_t>>> frameBuffers_;
};