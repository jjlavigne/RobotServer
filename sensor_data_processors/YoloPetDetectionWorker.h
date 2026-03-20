#pragma once
#include "SensorDataWorkerInterface.h"
#include <opencv2/dnn.hpp>
#include <opencv2/opencv.hpp>
#include <folly/ProducerConsumerQueue.h>

class YoloPetDetectionWorker : public SensorDataWorkerInterface {
public:
    YoloPetDetectionWorker(const std::string& modelPath);
    void start() override;
    void stop() override;
    void process(std::shared_ptr<SensorData> data) override;

private:
    void detectPets(cv::Mat& frame);
    cv::dnn::Net net;
    const int DOG_CLASS_ID = 16; // COCO Class ID for Dog
    const int CAT_CLASS_ID = 15; // COCO Class ID for Cat
    float confidenceThreshold = 0.45;
    folly::ProducerConsumerQueue<std::shared_ptr<SensorData>> queue_;
};