#include <iostream>
#include "StreamProcessor.h"

int main() {
    std::cout << "Hello, Robot Server!" << std::endl;
    StreamProcessor processor;
    auto data = std::make_shared<SensorData>();
    processor.process(data);
    return 0;
}