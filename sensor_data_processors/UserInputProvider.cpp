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

    //  Test renderer for the window
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
    }

    // 3. Main Loop Flag
    SDL_Event event;

    std::cout << "Controller started. Press W, A, S, D to drive. Close window to quit." << std::endl;

    auto sensorData = std::make_shared<SensorData>();
    sensorData->userInput = UserInputData();

    sensorData->userInput.value().forward = false;
    sensorData->userInput.value().backward = false;
    sensorData->userInput.value().left = false;
    sensorData->userInput.value().right = false;

    isRunning = true;

    // bool wasLedOn = false;

    // 4. The Event Loop
    while (isRunning) {
        
        // ==========================================
        // PHASE 1: INPUT (Process all pending events)
        // ==========================================
        while (SDL_PollEvent(&event) != 0) {   
            
            // Handle Key Pressed Down
            if (event.type == SDL_KEYDOWN) {
               if (event.key.repeat == 0) {
                    switch (event.key.keysym.sym) {
                        case SDLK_w: sensorData->userInput.value().forward = true; break;
                        case SDLK_a: sensorData->userInput.value().left = true; break;
                        case SDLK_s: sensorData->userInput.value().backward = true; break;
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
            // Optional: Handle user clicking the 'X' on the window
            else if (event.type == SDL_QUIT) {
                isRunning = false;
            }
        } // <--- IMPORTANT: The event loop MUST close here!

        // ==========================================
        // PHASE 2: UPDATE (Game logic & Network)
        // ==========================================
        bool forwardKeyPressed = sensorData->userInput.value().forward;
        bool backwardKeyPressed = sensorData->userInput.value().backward;
        bool leftKeyPressed = sensorData->userInput.value().left;
        bool rightKeyPressed = sensorData->userInput.value().right;

        // Note: Your comment says "Only send packet if the state CHANGED" 
        // but this logic still fires every single frame the key is held!
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

        // ==========================================
        // PHASE 3: RENDER (Draw the current frame)
        // ==========================================
        // A. Clear the screen (Black)
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // B. Define shape
        SDL_Rect myRect = { 640/4, 480/4, 640/2, 480/2 };

        // C. Set draw color (Red)
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

        // D. Draw rectangle
        SDL_RenderFillRect(renderer, &myRect);

        // E. Present to screen
        SDL_RenderPresent(renderer);
            
        // ==========================================
        // PHASE 4: WAIT (Control frame rate)
        // ==========================================
        SDL_Delay(16); // roughly 60 FPS
    }

    // Cleanup
    close(arduinoSocket);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
void UserInputProvider::stop() {
    isRunning = false;
}