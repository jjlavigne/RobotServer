#pragma once

#include "WorkerInterface.h"

#include <cstdint>
#include <memory>
#include <optional>
#include <vector>

struct UserInputData {
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

struct SensorData {
    std::optional<UserInputData> userInput;
    std::optional<ShapeData> shape;
    std::optional<ImageData> image;
    std::optional<std::vector<int>> detectedObjects;
};

// 1 == dog
// 2 == cat

enum class Object { Dog, Cat };

class SensorDataWorkerInterface : public WorkerInterface {
  public:
    virtual ~SensorDataWorkerInterface() = default;

    virtual void process(std::shared_ptr<SensorData> data) = 0;

    virtual void enqueue(std::shared_ptr<SensorData> data) = 0;
};

class Car {};

void asdfasdf() {
    SensorData data = SensorData();
    data.detectedObjects = {Object::Dog, Object::Cat};
}