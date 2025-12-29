#pragma once

#include <memory>
#include "SensorDataProcessorInterface.h"

struct SensorDataDispatch{};

class SensorDataDispatchInterface {
public:
  virtual ~SensorDataDispatchInterface() = default;

  virtual void enqueueData(std::shared_ptr<SensorData> data) = 0;
};