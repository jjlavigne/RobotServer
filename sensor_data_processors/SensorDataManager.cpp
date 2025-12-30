#include "SensorDataManager.h"
#include "UserInputProvider.h"

SensorDataManager::SensorDataManager(std::shared_ptr<SensorDataDispatcherInterface> dispatcher, std::vector<std::shared_ptr<SensorDataProviderInterface>> providers) : dispatcher_(std::move(dispatcher)), providers_(std::move(providers)) {}

void SensorDataManager::start(){

    for (auto& provider : providers_) {
        provider->start();
    }
};
