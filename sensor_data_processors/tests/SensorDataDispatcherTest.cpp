#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "SensorDataProcessorMock.h"
#include "SensorDataDispatcher.h"

using ::testing::StrictMock;
using ::testing::AtLeast;

class SensorDataDispatcherTest : public ::testing::Test {
  public:
    void SetUp() override {
        processorMock_ = std::make_shared<StrictMock<SensorDataProcessorMock>>();
        std::vector<std::shared_ptr<SensorDataProcessorInterface>> processors;
        processors.push_back(processorMock_);
        disPatcher_ = std::make_shared<SensorDataDispatcher>(processors);

    std::vector<std::shared_ptr<SensorDataProcessorInterface>> multiProcessors;
     for (int i = 0; i < numProcessors; i++) {
        auto processor = std::make_shared<StrictMock<SensorDataProcessorMock>>();
        multiProcessors_.push_back(processor);
        multiProcessors.push_back(processor);
     }

     multiDispatcher_ = std::make_shared<SensorDataDispatcher>(multiProcessors);

    }
    void TearDown() override {
        // Code here will be called immediately after each test (right
        // before the destructor).
    }

  std::shared_ptr<SensorDataProcessorMock> processorMock_;
  std::vector<std::shared_ptr<SensorDataProcessorMock>> multiProcessors_;
  std::shared_ptr<SensorDataDispatcher> disPatcher_;
  std::shared_ptr<SensorDataDispatcher> multiDispatcher_;
  int numProcessors = 3;
};

TEST_F(SensorDataDispatcherTest, ProcessTestSingleProcessor) {
    auto data = std::shared_ptr<SensorData>();

    // expect process is called on mock 1 time
    EXPECT_CALL(*processorMock_, process(data)).Times(1);

    disPatcher_->enqueueData(data);
}

TEST_F(SensorDataDispatcherTest, ProcessTestSingleProcessorMultipleCalls) {
    auto data = std::shared_ptr<SensorData>();
    int numCalls = 3;

    // expect process is called on mock 1 time
    EXPECT_CALL(*processorMock_, process(data)).Times(numCalls);

    for (int i = 0; i < numCalls ; ++i) {
        disPatcher_->enqueueData(data);
    }
}

TEST_F(SensorDataDispatcherTest, ProcessTestMultipleProcessors) {
    auto data = std::shared_ptr<SensorData>();

    // expect process is called on mock at least 1 time
    for (auto& processor : multiProcessors_) {
        EXPECT_CALL(*processor, process(data)).Times(3);
    }

    for (int i = 0; i < 3 ; ++i) {
        multiDispatcher_->enqueueData(data);
    }
}