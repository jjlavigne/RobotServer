#include "LLMPetDetectionWorker.h"
#include <chrono>
#include <cpr/cpr.h>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <thread>

using json = nlohmann::json;

std::string loadApiKey() {
    std::ifstream file("gemini_api_key.txt");
    if (!file.is_open()) {
        std::cerr << "CCould not open gemini_api_key.txt" << std::endl;
        return "";
    }
    std::string key;
    std::getline(file, key); // Safely read just the first line

    // Strip hidden Windows carriage returns (\r) and trailing spaces
    while (!key.empty() &&
           (key.back() == '\r' || key.back() == ' ' || key.back() == '\n')) {
        key.pop_back();
    }

    return key;
}

// Convert raw image bytes to Base64 string for the API
std::string base64_encode(const std::vector<uint8_t>& buf) {
    static const char base64_chars[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string ret;
    int i = 0, j = 0;
    uint8_t char_array_3[3], char_array_4[4];

    for (size_t k = 0; k < buf.size(); k++) {
        char_array_3[i++] = buf[k];
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) +
                              ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) +
                              ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;
            for (i = 0; (i < 4); i++)
                ret += base64_chars[char_array_4[i]];
            i = 0;
        }
    }
    if (i) {
        for (j = i; j < 3; j++)
            char_array_3[j] = '\0';
        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] =
            ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] =
            ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        for (j = 0; (j < i + 1); j++)
            ret += base64_chars[char_array_4[j]];
        while ((i++ < 3))
            ret += '=';
    }
    return ret;
}

LLMPetDetectionWorker::LLMPetDetectionWorker(
    std::shared_ptr<SensorDataDispatcherInterface> dispatcher)
    : dispatcher_(std::move(dispatcher)),
      queue_(folly::ProducerConsumerQueue<std::shared_ptr<SensorData>>(100)) {
    apiKey_ = loadApiKey();
}

void LLMPetDetectionWorker::enqueue(std::shared_ptr<SensorData> data) {
    if (!data->image.has_value()) {
        return;
    }
    queue_.write(data);
}

void LLMPetDetectionWorker::start() {
    std::cout << "LLMPetDetectionWorker started." << std::endl;
    isRunning_ = true;
    while (isRunning_) {
        if (!queue_.isEmpty()) {
            std::shared_ptr<SensorData> sensorData;
            if (queue_.read(sensorData)) {
                process(sensorData);
                std::this_thread::sleep_for(std::chrono::seconds(15));
            } else {
                std::cout << "Queue failed to read" << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }
    }
}

void LLMPetDetectionWorker::stop() {
    std::cout << "LLMPetDetectionWorker stopped." << std::endl;
    isRunning_ = false;
}

void LLMPetDetectionWorker::process(std::shared_ptr<SensorData> data) {
    if (!data->image.has_value() || data->image->jpegBuffer.empty()) {
        return;
    }

    // 1. Ask Gemini
    std::optional<Object> detectedPet =
        askGeminiForPet(data->image->jpegBuffer);

    // 2. If Gemini found something, update the SensorData struct
    if (detectedPet.has_value()) {

        // Initialize the vector if it's currently empty/nullopt
        if (!data->detectedObjects.has_value()) {
            data->detectedObjects = std::vector<int>();
        }

        // Cast the enum to an int and push it to the vector
        data->detectedObjects->push_back(static_cast<int>(detectedPet.value()));
    }
}

std::optional<Object> LLMPetDetectionWorker::askGeminiForPet(
    const std::vector<uint8_t>& imageBuffer) {
    if (apiKey_.empty())
        return std::nullopt;

    std::string url = "https://generativelanguage.googleapis.com/v1beta/models/"
                      "gemini-2.5-flash:generateContent?key=" +
                      apiKey_;
    std::string base64Image = base64_encode(imageBuffer);

    // 1. Force Gemini to reply with a strict JSON string map
    json payload = {
        {"contents",
         {{{"parts",
            {{{"text", "Examine this image. Is there a dog or a cat? Output "
                       "the exact JSON."}},
             {{"inline_data",
               {{"mime_type", "image/jpeg"}, {"data", base64Image}}}}}}}}},
        {"generationConfig",
         {{"response_mime_type", "application/json"},
          {"response_schema",
           {{"type", "OBJECT"},
            {"properties",
             {{"detectedAnimal",
               {
                   {"type", "STRING"},
                   {"enum", {"Dog", "Cat", "None"}} // <--- Gemini only knows
                                                    // about these strings
               }}}},
            {"required", json::array({"detectedAnimal"})}}}}}};

    cpr::Response r = cpr::Post(
        cpr::Url{url}, cpr::Header{{"Content-Type", "application/json"}},
        cpr::Body{payload.dump()});

    if (r.status_code == 200) {
        try {
            json apiResponse = json::parse(r.text);
            std::string aiJsonString =
                apiResponse["candidates"][0]["content"]["parts"][0]["text"]
                    .get<std::string>();
            json animalData = json::parse(aiJsonString);

            // Extract the string Gemini gave us
            std::string detectedText =
                animalData["detectedAnimal"].get<std::string>();

            // 2. THE BRIDGE: Translate the string into your C++ Enum
            if (detectedText == "Dog") {
                std::cout << "[LLM]: Dog found!" << std::endl;
                return Object::Dog;
            } else if (detectedText == "Cat") {
                std::cout << "[LLM]: Cat found!" << std::endl;
                return Object::Cat;
            } else {
                std::cout << "[LLM]: Neither found." << std::endl;
                return std::nullopt;
            }

        } catch (const std::exception& e) {
            std::cerr << "[LLM ERROR] Parsing failed: " << e.what()
                      << std::endl;
            return std::nullopt;
        }
    } else {
        std::cerr << "[LLM ERROR] HTTP " << r.status_code << std::endl;
        std::cerr << "Google says: " << r.text << std::endl;
        return std::nullopt;
    }
}