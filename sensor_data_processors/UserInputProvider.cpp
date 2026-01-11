#include "UserInputProvider.h"
#include <iostream>
#include <thread>
#include <SDL2/SDL.h>
#include "SensorDataProcessorInterface.h"


#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h> 


const char* ARDUINO_IP = "192.168.7.23"; 
const int ARDUINO_PORT = 4210;

void UserInputProvider::start() {
    int arduinoSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (arduinoSocket < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return;
    }

    struct sockaddr_in arduinoAddr;
    arduinoAddr.sin_family = AF_INET;
    arduinoAddr.sin_port = htons(ARDUINO_PORT);
    inet_pton(AF_INET, ARDUINO_IP, &arduinoAddr.sin_addr);

    auto sendUDP = [&](const char* message) {
        std::cout << "UDP: " << message << std::endl;
        sendto(arduinoSocket, message, strlen(message), 0, 
              (struct sockaddr*)&arduinoAddr, sizeof(arduinoAddr));
    };

    // 1. Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
    }

    // 2. Create a window (Required to capture keyboard events)
    SDL_Window* window = SDL_CreateWindow(
        "Robot Controller (click here)",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        640,
        480,
        SDL_WINDOW_SHOWN
    );

    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
    }

    // 3. Main Loop Flag
    bool isRunning = true;
    SDL_Event event;

    std::cout << "Controller started. Press W, A, S, D to drive. Close window to quit." << std::endl;

    auto sensorData = std::make_shared<SensorData>();
    sensorData->userInput = UserInputData();

    sensorData->userInput.value().forward = false;
    sensorData->userInput.value().backward = false;
    sensorData->userInput.value().left = false;
    sensorData->userInput.value().right = false;

    // bool wasLedOn = false;

    // 4. The Event Loop
    while (isRunning) {
        // Poll for events (keyboard, mouse, window close, etc.)
        while (SDL_PollEvent(&event) != 0) {
            
            // Handle Quit (Esc button on window)
            if (event.type == SDL_QUIT) {
                isRunning = false;
            }
            
            // Handle Key Pressed Down
            else if (event.type == SDL_KEYDOWN) {
                // 'repeat' checks if this is a hold-down repeat. 
                // We usually ignore repeats for robot logic to avoid spamming commands.
               if (event.key.repeat == 0) {
                    switch (event.key.keysym.sym) {
                        case SDLK_w: sensorData->userInput.value().forward = true; break;
                        case SDLK_a: sensorData->userInput.value().left = true; break; // 'a' usually maps to Left, but keeping your logic
                        case SDLK_s: sensorData->userInput.value().backward = true; break;     // 's' usually maps to Backward
                        case SDLK_d: sensorData->userInput.value().right = true; break;
                        case SDLK_ESCAPE: isRunning = false; break;
                    }
                }
            }

            // Handle Key Released
            else if (event.type == SDL_KEYUP) {
                switch (event.key.keysym.sym) {
                    case SDLK_w: sensorData->userInput.value().forward = false; break;
                    case SDLK_a: sensorData->userInput.value().left = false; break;
                    case SDLK_s: sensorData->userInput.value().backward = false; break;
                    case SDLK_d: sensorData->userInput.value().right = false; break;
                }
            }

            //UDP Logic to check if ANY key is currently pressed 
            bool forwardKeyPressed = sensorData->userInput.value().forward;
            bool backwardKeyPressed = sensorData->userInput.value().backward;
            bool leftKeyPressed = sensorData->userInput.value().left;
            bool rightKeyPressed = sensorData->userInput.value().right;

           // Only send packet if the state CHANGED (OFF -> ON or ON -> OFF)
            if (forwardKeyPressed) {
                sendUDP("MOTOR, FORWARD");
                dispatcher_->enqueueData(sensorData); 
            } 
            else if (backwardKeyPressed) {
                sendUDP("MOTOR, BACKWARD");
                dispatcher_->enqueueData(sensorData);  
            }
            else if (leftKeyPressed) {
                sendUDP("MOTOR, LEFT");
                dispatcher_->enqueueData(sensorData);  
            }
            else if (rightKeyPressed) {
                sendUDP("MOTOR, RIGHT");
                dispatcher_->enqueueData(sensorData);  
            }
        }
    }

    // Cleanup
    close(arduinoSocket);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void UserInputProvider::stop() {
    SDL_Quit();
}