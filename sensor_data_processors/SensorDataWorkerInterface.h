#pragma once

#include "WorkerInterface.h"

#include <memory>
#include <optional>

struct UserInputData{
  bool forward = false;
  bool backward = false;
  bool left = false;
  bool right = false; 
};

struct SensorData{
  std::optional<UserInputData> userInput;
};

class SensorDataWorkerInterface : public WorkerInterface {
public:
  virtual ~SensorDataWorkerInterface() = default;

  virtual void process(std::shared_ptr<SensorData> data) = 0;
};

