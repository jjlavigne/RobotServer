#pragma once

#include <memory>

class WorkerInterface {
public:
  virtual ~WorkerInterface() = default;

  virtual void start() = 0;
  virtual void stop() = 0;
};