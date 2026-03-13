#pragma once

#include "SensorDataWorkerInterface.h"

// Responsible for processing user input and sending commands to the robot

class UserInputProcessorWorker : public SensorDataWorkerInterface {
    public: 
    void start() override;
    void stop() override;
    void process(std::shared_ptr<SensorData> data) override;
};
