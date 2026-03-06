#include <iostream>
#include <ncurses.h>
#include "StreamProcessor.h"
#include "SensorDataManager.h"
#include "SensorDataDispatcher.h"
#include "UserInputProvider.h"
#include "UserInputProcessor.h"
#include "RemoteWebcamProvider.h"


int main() {
    std::cout << "Hello, Robot Server!" << std::endl;
    std::vector<std::shared_ptr<SensorDataProcessorInterface>> processors;
    auto processor = std::make_shared<UserInputProcessor>();
    processors.push_back(processor);
    auto dispatcher = std::make_shared<SensorDataDispatcher>(processors);

    std::vector<std::shared_ptr<SensorDataProviderInterface>> providers;
    auto inputProvider = std::make_shared<UserInputProvider>(dispatcher);
    auto remoteWebcamProvider = std::make_shared<RemoteWebcamProvider>(dispatcher);
    providers.push_back(inputProvider);
    providers.push_back(remoteWebcamProvider);

    SensorDataManager sensorDataManager(dispatcher, providers);
    sensorDataManager.start();
    
    char input;
    std::cin >> input;
    while (input != 'q') {
        std::cin >> input;
    }

    sensorDataManager.stop();

    std::cout << "Bye robot Server!" << std::endl;

    // TODO: write code that blocks and waits for input to end the program
    // when key is pressed, call sensorDataMAnager.stop

    
    return 0;
}