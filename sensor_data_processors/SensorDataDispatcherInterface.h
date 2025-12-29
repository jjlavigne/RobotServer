#include <memory>

struct SensorDataDispatch{};

class SensorDataDispatchInterface {
public:
  virtual ~SensorDataDispatchInterface() = default;

  virtual void enqueueData(std::shared_ptr<SensorDataDispatch> data) = 0;
};