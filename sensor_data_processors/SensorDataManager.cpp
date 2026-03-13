#include "SensorDataManager.h"
#include "SDLWorker.h"

SensorDataManager::SensorDataManager(std::shared_ptr<SensorDataDispatcherInterface> dispatcher, std::vector<std::shared_ptr<WorkerInterface>> providers) : dispatcher_(std::move(dispatcher)), providers_(std::move(providers)) {}

void SensorDataManager::start(){

    for (auto& provider : providers_) {
        std::thread worker([&provider]() { provider->start(); });
        workerThreads_.push_back(std::move(worker));
    }
}
void SensorDataManager::stop(){
    for (auto& provider : providers_) {
        provider->stop();
    }
    for (auto& worker : workerThreads_) {
        worker.join();
    }
}

