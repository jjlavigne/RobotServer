#pragma once
#include "SensorDataDispatcher.h"
#include "SensorDataWorkerInterface.h"
#include <atomic>
#include <folly/ProducerConsumerQueue.h>
#include <memory>
#include <opencv2/opencv.hpp>

class PetDetectionWorker : public SensorDataWorkerInterface {
  public:
    PetDetectionWorker();
    ~PetDetectionWorker() override = default;

    void start() override;
    void stop() override;
    void process(std::shared_ptr<SensorData> data) override;
    void enqueue(std::shared_ptr<SensorData> ddata) override;

  private:
    cv::dnn::Net net; // The "brain" variable
    void detectDogs(cv::Mat& frame);
    folly::ProducerConsumerQueue<std::shared_ptr<SensorData>> queue_;
    std::atomic<bool> isRunning_{true};
};