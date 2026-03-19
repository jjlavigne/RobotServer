#pragma once

#include "WorkerInterface.h"

#include <memory>
#include <optional>
#include <vector>
#include <cstdint>

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

struct ImageData {
    std::vector<uint8_t> jpegBuffer;
};

struct SensorData{
  std::optional<UserInputData> userInput;
  std::optional<ShapeData> shape;
  std::optional<ImageData> image;
  
};

class SensorDataWorkerInterface : public WorkerInterface {
public:
  virtual ~SensorDataWorkerInterface() = default;

  virtual void process(std::shared_ptr<SensorData> data) = 0;
};

