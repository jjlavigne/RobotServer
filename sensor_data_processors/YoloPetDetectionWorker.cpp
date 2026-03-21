#include "YoloPetDetectionWorker.h"
#include <iostream>

YoloPetDetectionWorker::YoloPetDetectionWorker(const std::string& modelPath)
    : queue_(100) {
    std::cout << "[YOLO] Loading YOLOv26 Pet Detection Model..." << std::endl;
    net = cv::dnn::readNetFromONNX(modelPath);

    // Use CUDA if available, otherwise CPU
    net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
}

void YoloPetDetectionWorker::start() {
    std::cout << "YoloPetDetectionWorker started." << std::endl;
    isRunning_ = true;
    int numFramesSinceLastProcessed = 0;
    while (isRunning_) {
        // std::cout << queue_.sizeGuess() << std::endl;
        if (!queue_.isEmpty()) {
            std::shared_ptr<SensorData> sensorData;
            if (queue_.read(sensorData)) {
                if (numFramesSinceLastProcessed > 4) {
                    process(sensorData);
                    numFramesSinceLastProcessed = 0;
                }
                ++numFramesSinceLastProcessed;
            } else {
                std::cout << "Queue failed to read" << std::endl;
            }
        }
    }
}

void YoloPetDetectionWorker::stop() {
    std::cout << "YoloPetDetectionWorker stopped." << std::endl;
    isRunning_ = false;
}

void YoloPetDetectionWorker::process(std::shared_ptr<SensorData> data) {
    if (!data || !data->image.has_value() || data->image->jpegBuffer.empty())
        return;

    // Decode the JPEG buffer shared from the RemoteWebcamProvider
    cv::Mat frame = cv::imdecode(data->image->jpegBuffer, cv::IMREAD_COLOR);
    if (frame.empty())
        return;

    detectPets(frame);
}

void YoloPetDetectionWorker::enqueue(std::shared_ptr<SensorData> data) {
    if (!data->image.has_value()) {
        return;
    }
    queue_.write(data);
}

// void YoloPetDetectionWorker::detectPets(cv::Mat& frame) {
//     // 1. Pre-process: 640x640, scale 1/255, swap BGR to RGB
//     cv::Mat blob = cv::dnn::blobFromImage(frame, 1 / 255.0, cv::Size(640,
//     640),
//                                           cv::Scalar(0, 0, 0), true, false);
//     net.setInput(blob);

//     // 2. Inference
//     std::vector<cv::Mat> outputs;
//     net.forward(outputs, net.getUnconnectedOutLayersNames());

//     cv::Mat output = outputs[0];

//     // Safety: Flatten the [1, 300, 6] tensor into a [300, 6] matrix
//     if (output.dims == 3) {
//         int sz[] = {output.size[1], output.size[2]};
//         output = cv::Mat(2, sz, CV_32F, output.ptr<float>());
//     }

//     // Scaling factors to map 640x640 detections back to original frame size
//     float scaleX = (float)frame.cols / 640.0f;
//     float scaleY = (float)frame.rows / 640.0f;

//     for (int i = 0; i < output.rows; ++i) {
//         // E2E Row Format: [x1, y1, x2, y2, score, class_id]
//         float confidence = output.at<float>(i, 4);

//         if (confidence > confidenceThreshold) {
//             int classId = static_cast<int>(output.at<float>(i, 5));

//             // Standard COCO: 15 = Cat, 16 = Dog
//             if (classId == DOG_CLASS_ID || classId == CAT_CLASS_ID) {
//                 // Coordinates are already Top-Left and Bottom-Right
//                 float x1 = output.at<float>(i, 0) * scaleX;
//                 float y1 = output.at<float>(i, 1) * scaleY;
//                 float x2 = output.at<float>(i, 2) * scaleX;
//                 float y2 = output.at<float>(i, 3) * scaleY;

//                 std::string label = (classId == DOG_CLASS_ID) ? "DOG" :
//                 "CAT";

//                 // Logging the detection
//                 std::cout << ">>> " << label
//                           << " DETECTED! Confidence: " << (confidence * 100)
//                           << "% "
//                           << "at [" << x1 << ", " << y1 << "]" << std::endl;
//             }
//         }
//     }
// }

void YoloPetDetectionWorker::detectPets(cv::Mat& frame) {
    static const std::vector<std::string> classNames = {
        "person",        "bicycle",      "car",
        "motorcycle",    "airplane",     "bus",
        "train",         "truck",        "boat",
        "traffic light", "fire hydrant", "stop sign",
        "parking meter", "bench",        "bird",
        "cat",           "dog",          "horse",
        "sheep",         "cow",          "elephant",
        "bear",          "zebra",        "giraffe",
        "backpack",      "umbrella",     "handbag",
        "tie",           "suitcase",     "frisbee",
        "skis",          "snowboard",    "sports ball",
        "kite",          "baseball bat", "baseball glove",
        "skateboard",    "surfboard",    "tennis racket",
        "bottle",        "wine glass",   "cup",
        "fork",          "knife",        "spoon",
        "bowl",          "banana",       "apple",
        "sandwich",      "orange",       "broccoli",
        "carrot",        "hot dog",      "pizza",
        "donut",         "cake",         "chair",
        "couch",         "potted plant", "bed",
        "dining table",  "toilet",       "tv",
        "laptop",        "mouse",        "remote",
        "keyboard",      "cell phone",   "microwave",
        "oven",          "toaster",      "sink",
        "refrigerator",  "book",         "clock",
        "vase",          "scissors",     "teddy bear",
        "hair drier",    "toothbrush"};

    cv::Mat blob = cv::dnn::blobFromImage(frame, 1 / 255.0, cv::Size(640, 640),
                                          cv::Scalar(0, 0, 0), true, false);
    net.setInput(blob);

    std::vector<cv::Mat> outputs;
    net.forward(outputs, net.getUnconnectedOutLayersNames());
    cv::Mat output = outputs[0];

    if (output.dims == 3) {
        int sz[] = {output.size[1], output.size[2]};
        output = cv::Mat(2, sz, CV_32F, output.ptr<float>());
    }

    // Header for the frame's detections
    std::cout << "\n--- New Frame Analysis ---" << std::endl;

    for (int i = 0; i < output.rows; ++i) {
        float confidence = output.at<float>(i, 4);

        // Setting this to 0.30 catches almost everything clearly visible
        if (confidence > 0.30) {
            int classId = static_cast<int>(output.at<float>(i, 5));
            std::string label =
                (classId < classNames.size()) ? classNames[classId] : "unknown";

            // Print EVERY object found above the threshold
            std::cout << "[FOUND] " << label << " (" << (confidence * 100)
                      << "%)" << std::endl;

            // Keep your specific pet logic active for the robot's "brain"
            if (classId == 16 || classId == 15) { // 16=Dog, 15=Cat
                std::cout << ">>> CRITICAL: Pet target identified!"
                          << std::endl;
            }
        }
    }
}