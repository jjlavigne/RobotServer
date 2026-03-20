#include "YoloPetDetectionWorker.h"
#include <iostream>

YoloPetDetectionWorker::YoloPetDetectionWorker(const std::string& modelPath) : queue_(100) {
    std::cout << "[YOLO] Loading YOLOv5 Pet Detection Model..." << std::endl;
    net = cv::dnn::readNetFromONNX(modelPath);
    
    // Use CUDA if available, otherwise CPU
    net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU); 
}

void YoloPetDetectionWorker::start() {
    std::cout << "YoloPetDetectionWorker started." << std::endl;
}

void YoloPetDetectionWorker::stop() {
    std::cout << "YoloPetDetectionWorker stopped." << std::endl;
}

void YoloPetDetectionWorker::process(std::shared_ptr<SensorData> data) {
    if (!data || !data->image.has_value() || data->image->jpegBuffer.empty()) return;

    // Decode the JPEG buffer shared from the RemoteWebcamProvider
    cv::Mat frame = cv::imdecode(data->image->jpegBuffer, cv::IMREAD_COLOR);
    if (frame.empty()) return;

    detectPets(frame);
}

void YoloPetDetectionWorker::detectPets(cv::Mat& frame) {
    // 1. Pre-process
    cv::Mat blob = cv::dnn::blobFromImage(frame, 1/255.0, cv::Size(640, 640), cv::Scalar(0,0,0), true, false);
    net.setInput(blob);

    // 2. Inference
    std::vector<cv::Mat> outputs;
    net.forward(outputs, net.getUnconnectedOutLayersNames());

    cv::Mat output = outputs[0];
    
    // Fix the 3D tensor shape crash
    if (output.dims == 3) {
        int sz[] = { output.size[1], output.size[2] };
        output = cv::Mat(2, sz, CV_32F, output.ptr<float>());
    }

    cv::transpose(output, output);

    // Data structures to hold candidates before filtering
    std::vector<int> class_ids;
    std::vector<float> confidences;
    std::vector<cv::Rect> boxes;

    for (int i = 0; i < output.rows; ++i) {
        cv::Mat row = output.row(i);
        cv::Mat scores = row.colRange(4, row.cols);
        cv::Point classIdPoint;
        double confidence;
        cv::minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);

        if (confidence > confidenceThreshold) {
            int classId = classIdPoint.x;
            if (classId == DOG_CLASS_ID || classId == CAT_CLASS_ID) {
                // YOLOv8/12 coords are: [center_x, center_y, width, height]
                float x = row.at<float>(0);
                float y = row.at<float>(1);
                float w = row.at<float>(2);
                float h = row.at<float>(3);

                // Convert to top-left corner for OpenCV Rect
                int left = static_cast<int>(x - w/2);
                int top = static_cast<int>(y - h/2);
                
                boxes.push_back(cv::Rect(left, top, static_cast<int>(w), static_cast<int>(h)));
                confidences.push_back(static_cast<float>(confidence));
                class_ids.push_back(classId);
            }
        }
    }

    // 3. Non-Maximum Suppression (The key to stopping the "flood" of detections)
    std::vector<int> indices;
    cv::dnn::NMSBoxes(boxes, confidences, confidenceThreshold, 0.45f, indices);

    for (int idx : indices) {
        std::string label = (class_ids[idx] == DOG_CLASS_ID) ? "DOG" : "CAT";
        std::cout << "!!! " << label << " CONFIRMED !!! Confidence: " 
                  << (confidences[idx] * 100) << "%" << std::endl;
    }
}