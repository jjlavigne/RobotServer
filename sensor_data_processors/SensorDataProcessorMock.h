#pragma once

#include <gmock/gmock.h>
#include "SensorDataWorkerInterface.h"

class SensorDataProcessorMock : public SensorDataWorkerInterface{
  public:
    SensorDataProcessorMock();
    ~SensorDataProcessorMock() override;

    MOCK_METHOD(void, process, (std::shared_ptr<SensorData> data), (override));

};