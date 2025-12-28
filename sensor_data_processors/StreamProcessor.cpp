#include <iostream>
#include "StreamProcessor.h"

void StreamProcessor::process(std::shared_ptr<SensorData> data) {
    std::cout << "we did it!" << std::endl;
}