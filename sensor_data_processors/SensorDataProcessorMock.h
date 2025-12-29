#pragma once

#include <gmock/gmock.h>
#include "SensorDataProcessorInterface.h"

class SensorDataProcessorMock : public SensorDataProcessorInterface{
  public:
    SensorDataProcessorMock();
    ~SensorDataProcessorMock() override;

    MOCK_METHOD(void, process, (std::shared_ptr<SensorData> data), (override));

};