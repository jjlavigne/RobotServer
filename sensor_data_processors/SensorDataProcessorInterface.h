#pragma once

#include <memory>
#include <optional>

struct UserInputData{
  bool forward = false;
  bool backward = false;
  bool left = false;
  bool right = false; 
};

struct SensorData{
};

class SensorDataProcessorInterface {
public:
  virtual ~SensorDataProcessorInterface() = default;

  virtual void process(std::shared_ptr<SensorData> data) = 0;
};

