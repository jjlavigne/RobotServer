#include "PetDetectionWorker.h"
#include <iostream>
#include <opencv2/dnn.hpp>      
#include <opencv2/imgproc.hpp>  

PetDetectionWorker::PetDetectionWorker() { 
    std::cout << "Loading Pet Detection Model..." << std::endl;
    try {
        // Files must be in the same folder as your executable
        std::string protoPath = std::string(MODEL_DIR) + "deploy.prototxt";
        std::string modelPath = std::string(MODEL_DIR) + "mobilenet_iter_73000.caffemodel";

        net = cv::dnn::readNetFromCaffe(protoPath, modelPath);
        if (net.empty()) {
            std::cerr << "Critical Error: Model net is empty!" << std::endl;
        } else {
            std::cout << "Model loaded successfully!" << std::endl;
        }
    } catch (const cv::Exception& e) {
        std::cerr << "Error loading model: " << e.what() << std::endl;
    }
}
    
void PetDetectionWorker::start() {
    std::cout << "PetDetectionWorker started." << std::endl;
}

void PetDetectionWorker::stop() {
    std::cout << "PetDetectionWorker stopped." << std::endl;
}

void PetDetectionWorker::process(std::shared_ptr<SensorData> data) {
    //std::cout << "PetDetectionWorker process entered" << std::endl;
    if (!data->image.has_value()) {
        //std::cout << "Data did not have a value" << std::endl;
        return;
    }

    cv::Mat frame = cv::imdecode(data->image->jpegBuffer, cv::IMREAD_COLOR);
    //std::cout << "Frame indecoded" << std::endl;

    if (frame.empty()) {
        std::cerr << "Failed to decode JPEG frame!" << std::endl;
        return;
    }

    std::cout << "About to call detectdods()" << std::endl;
    detectDogs(frame);
}

void PetDetectionWorker::detectDogs(cv::Mat& frame) {
    //std::cout << "PetDetectionWorker detectDogs entered." << std::endl;

    if (net.empty()) return;

    // 1. Prepare the image for the Neural Network (blobbing)
    cv::Mat blob = cv::dnn::blobFromImage(frame, 0.007843, cv::Size(300, 300), 127.5);
    net.setInput(blob);

    // 2. Run the detection
    cv::Mat detections = net.forward();

    // 3. Loop through the results
    cv::Mat detectionMat(detections.size[2], detections.size[3], CV_32F, detections.ptr<float>());

    for (int i = 0; i < detectionMat.rows; i++) {
        float confidence = detectionMat.at<float>(i, 2);

        if (confidence > 0.5) { // 50% threshold
            int classId = static_cast<int>(detectionMat.at<float>(i, 1));
            
            // Class ID 12 is 'Dog' in MobileNet-SSD
            if (classId == 12) {
                std::cout << "!!! DOG DETECTED !!! Confidence: " << confidence * 100 << "%" << std::endl;
                
                // Optional: Draw a box around the dog for debugging
                // int xLeftBottom = static_cast<int>(detectionMat.at<float>(i, 3) * frame.cols);
                // int yLeftBottom = static_cast<int>(detectionMat.at<float>(i, 4) * frame.rows);
                // cv::rectangle(frame, cv::Rect(xLeftBottom, yLeftBottom, 100, 100), cv::Scalar(0, 255, 0));
            }
        }
    }
}