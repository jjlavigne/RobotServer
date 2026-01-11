#pragma once

#include "SensorDataProcessorInterface.h"

class UserInputProcessor : public SensorDataProcessorInterface {
    public: 
    void process(std::shared_ptr<SensorData> data) override;
    
};
