#pragma once

#include "SensorDataWorkerInterface.h"
#include <folly/ProducerConsumerQueue.h>

// Responsible for processing user input and sending commands to the robot

class UserInputProcessorWorker : public SensorDataWorkerInterface {
    public: 
        UserInputProcessorWorker();
        ~UserInputProcessorWorker() override = default;
        void start() override;
        void stop() override;
        void process(std::shared_ptr<SensorData> data) override;
        void enqueue(std::shared_ptr<SensorData> data) override;

    private:
        folly::ProducerConsumerQueue<std::shared_ptr<SensorData>> queue_;
        bool isRunning_ = true;
};
