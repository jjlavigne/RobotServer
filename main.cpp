#include <iostream>
#include "StreamProcessor.h"
#include "WorkerManager.h"
#include "SensorDataDispatcher.h"
#include "SDLWorker.h"
#include "UserInputProcessorWorker.h"
#include "RemoteWebcamProviderWorker.h"
#include "SimulatedWebcamSender.h"
#include "PetDetectionWorker.h"

int main() {
    std::cout << "Hello, Robot Server!" << std::endl;
    std::vector<std::shared_ptr<SensorDataWorkerInterface>> processors;
    auto userInputProcessor = std::make_shared<UserInputProcessorWorker>();
    processors.push_back(userInputProcessor);
    auto dispatcher = std::make_shared<SensorDataDispatcher>(processors);

    std::vector<std::shared_ptr<WorkerInterface>> workers;
    std::vector<std::shared_ptr<WorkerInterface>> asyncWorkers;
    auto sdlWorker = std::make_shared<SDLWorker>(dispatcher);
    auto remoteWebcamProviderWorker = std::make_shared<RemoteWebcamProviderWorker>(dispatcher);
    auto petDetectionWorker = std::make_shared<PetDetectionWorker>();
    //auto simulatedWebcamSender = std::make_shared<SimulatedWebcamSender>();

    dispatcher->addProcessor(sdlWorker);

    asyncWorkers.push_back(remoteWebcamProviderWorker);
    asyncWorkers.push_back(sdlWorker);
    asyncWorkers.push_back(petDetectionWorker);
    //asyncWorkers.push_back(simulatedWebcamSender);

    WorkerManager workerManager(dispatcher, workers, asyncWorkers);
    workerManager.start();
    
    char input;
    std::cin >> input;
    while (input != 'q') {
        std::cin >> input;
    }

    workerManager.stop();

    std::cout << "Bye robot Server!" << std::endl;

    // TODO: write code that blocks and waits for input to end the program
    // when key is pressed, call workerManager.stop

    
    return 0;
}