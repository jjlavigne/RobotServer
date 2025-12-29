#include "SensorDataDispatcherInterface.h"

class SensorDataDispatcher : public SensorDataDispatchInterface{
    public: 
    void enqueueData(std::shared_ptr<SensorDataDispatch> data);
};
