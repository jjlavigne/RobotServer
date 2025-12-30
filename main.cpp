#include <iostream>
#include "StreamProcessor.h"
#include "SensorDataManager.h"
#include "SensorDataDispatcher.h"
#include "UserInputProvider.h"


int main() {
    std::cout << "Hello, Robot Server!" << std::endl;
    std::vector<std::shared_ptr<SensorDataProcessorInterface>> processors;
    auto dispatcher = std::make_shared<SensorDataDispatcher>(processors);
    
    std::vector<std::shared_ptr<SensorDataProviderInterface>> providers;
    auto inputProvider = std::make_shared<UserInputProvider>(dispatcher);
    providers.push_back(inputProvider);

    SensorDataManager sensorDataManager(dispatcher, providers);
    sensorDataManager.start();
    std::cout << "Bye robot Server!" << std::endl;

    return 0;
}