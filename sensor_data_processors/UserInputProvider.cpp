#include "UserInputProvider.h"
#include <iostream>
#include <thread>
#include <SDL2/SDL.h>

void UserInputProvider::start() {
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
                        case SDLK_w: std::cout << "pressed w" << std::endl; break;
                        case SDLK_a: break;
                        case SDLK_s: break;
                        case SDLK_d: break;
                        case SDLK_ESCAPE: isRunning = false; break;
                    }
                }
            }

            // Handle Key Released
            else if (event.type == SDL_KEYUP) {
                switch (event.key.keysym.sym) {
                    case SDLK_w: std::cout << "released w" << std::endl; break;
                    case SDLK_a:  break;
                    case SDLK_s:  break;
                    case SDLK_d:  break;
                }
            }
        }
    }
}

void UserInputProvider::stop() {
    
}