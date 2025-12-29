#include <memory>
#include <optional>

struct SensorData{
};

class SensorDataProcessorInterface {
public:
  virtual ~SensorDataProcessorInterface() = default;

  virtual void process(std::shared_ptr<SensorData> data) = 0;
};

