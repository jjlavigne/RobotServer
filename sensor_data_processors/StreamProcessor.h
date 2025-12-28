#include "SensorDataProcessorsInterface.h"

class StreamProcessor : public SensorDataProcessorsInterface{
    public: 
    void process(std::shared_ptr<SensorData> data);
    
};
