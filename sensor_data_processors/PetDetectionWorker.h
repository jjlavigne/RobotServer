#pragma once
#include "SensorDataWorkerInterface.h"
#include "SensorDataDispatcher.h"
#include <atomic>
#include <memory>
#include <opencv2/opencv.hpp>

class PetDetectionWorker : public SensorDataWorkerInterface {
public: 
    PetDetectionWorker() = default;
    ~PetDetectionWorker() override = default;

    void start() override;
    void stop() override;
    void process(std::shared_ptr<SensorData> data) override;
};