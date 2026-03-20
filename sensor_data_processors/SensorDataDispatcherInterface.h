#pragma once

#include "SensorDataWorkerInterface.h"
#include <memory>

struct SensorDataDispatch {};

class SensorDataDispatcherInterface {
  public:
    virtual ~SensorDataDispatcherInterface() = default;

    virtual void enqueueData(std::shared_ptr<SensorData> data) = 0;
};