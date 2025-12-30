#pragma once

#include <memory>

class SensorDataProviderInterface {
public:
  virtual ~SensorDataProviderInterface() = default;

  virtual void start() = 0;
  virtual void stop() = 0;
};