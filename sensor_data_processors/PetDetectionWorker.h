#pragma once
#include "SensorDataWorkerInterface.h"
#include "SensorDataDispatcher.h"
#include <atomic>
#include <memory>
#include <opencv2/opencv.hpp>

class PetDetectionWorker : public SensorDataWorkerInterface {
public: 
    PetDetectionWorker();
    ~PetDetectionWorker() override = default;

    void start() override;
    void stop() override;
    void process(std::shared_ptr<SensorData> data) override;
private: 
    cv::dnn::Net net; // The "brain" variable
    void detectDogs(cv::Mat& frame);
};