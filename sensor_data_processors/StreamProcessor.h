#pragma once

#include "SensorDataProcessorInterface.h"

class StreamProcessor : public SensorDataProcessorInterface{
    public: 
    void process(std::shared_ptr<SensorData> data) override;
    
};
