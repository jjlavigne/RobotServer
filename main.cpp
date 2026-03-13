#include <iostream>
#include <ncurses.h>
#include "StreamProcessor.h"
#include "SensorDataManager.h"
#include "SensorDataDispatcher.h"
#include "SDLWorker.h"
#include "UserInputProcessorWorker.h"
#include "RemoteWebcamProviderWorker.h"
#include "SimulatedWebcamSender.h"


int main() {
    std::cout << "Hello, Robot Server!" << std::endl;
    std::vector<std::shared_ptr<SensorDataWorkerInterface>> processors;
    auto processor = std::make_shared<UserInputProcessorWorker>();
    processors.push_back(processor);
    auto dispatcher = std::make_shared<SensorDataDispatcher>(processors);

    std::vector<std::shared_ptr<WorkerInterface>> providers;
    auto inputProvider = std::make_shared<SDLWorker>(dispatcher);
    auto remoteWebcamProviderWorker = std::make_shared<RemoteWebcamProviderWorker>(dispatcher);
    auto simulatedWebcamSender = std::make_shared<SimulatedWebcamSender>();
    providers.push_back(inputProvider);
    providers.push_back(remoteWebcamProviderWorker);
    // providers.push_back(simulatedWebcamSender);

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