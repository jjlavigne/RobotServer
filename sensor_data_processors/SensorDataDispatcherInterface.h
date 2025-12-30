#pragma once

#include <memory>
#include "SensorDataProcessorInterface.h"

struct SensorDataDispatch{};

class SensorDataDispatcherInterface {
public:
  virtual ~SensorDataDispatcherInterface() = default;

  virtual void enqueueData(std::shared_ptr<SensorData> data) = 0;
};