#pragma once

#include "SensorDataWorkerInterface.h"

class StreamProcessor : public SensorDataWorkerInterface{
    public: 
    void process(std::shared_ptr<SensorData> data) override;
    
};
