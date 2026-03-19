#include "PetDetectionWorker.h"
    
void PetDetectionWorker::start() {
}

void PetDetectionWorker::stop() {

}

void PetDetectionWorker::process(std::shared_ptr<SensorData> data) {
    if (!data->image.has_value())
}