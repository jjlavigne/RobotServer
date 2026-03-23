#pragma once
#include "SensorDataDispatcher.h"
#include "SensorDataWorkerInterface.h"
#include <atomic>
#include <folly/ProducerConsumerQueue.h>
#include <memory>
#include <opencv2/opencv.hpp>

class SavedDogFrames : public SensorDataWorkerInterface {
  public:
    SavedDogFrames();
    ~SavedDogFrames() override = default;

    void start() override;
    void stop() override;
    void process(std::shared_ptr<SensorData> data) override;
    void enqueue(std::shared_ptr<SensorData> ddata) override;

  private:
    folly::ProducerConsumerQueue<std::shared_ptr<SensorData>> queue_;
    bool isRunning_ = true;
};