#pragma once

#include "WorkerInterface.h"

#include <memory>
#include <optional>
#include <vector>

struct UserInputData{
  bool forward = false;
  bool backward = false;
  bool left = false;
  bool right = false; 
};

struct ShapeData {
    int x, y, width, height;
    int r, g, b; // Added color specifications!
};

struct SensorData{
  std::optional<UserInputData> userInput;
  std::optional<ShapeData> shape;
  
};

class SensorDataWorkerInterface : public WorkerInterface {
public:
  virtual ~SensorDataWorkerInterface() = default;

  virtual void process(std::shared_ptr<SensorData> data) = 0;
};

