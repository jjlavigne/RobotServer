#include <memory>

struct SensorData{};

class SensorDataProcessorsInterface {
public:
  virtual ~SensorDataProcessorsInterface() = default;

  virtual void process(std::shared_ptr<SensorData> data) = 0;
};

