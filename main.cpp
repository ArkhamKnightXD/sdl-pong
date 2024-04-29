#include <iostream>
#include "inc/SDL.h"

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Rect player1;
SDL_Rect player2;
SDL_Rect ball;

const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 544;
const int FRAME_RATE = 60; // Desired frame rate (frames per second)

// Function to handle events
void handleEvents() {

    SDL_Event event;

    while (SDL_PollEvent(&event)) {

        if (event.type == SDL_QUIT) {
            
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
            exit(0);
        }
    }
}

// Function to update rectangle movement.
void update() {

    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

    if (player1.y > 0 && currentKeyStates[SDL_SCANCODE_W]) {
        player1.y -= 10;
    }

    else if (player1.y < SCREEN_HEIGHT - 64 && currentKeyStates[SDL_SCANCODE_S]) {
        player1.y += 10;
    }

    if (player2.y > 0 && currentKeyStates[SDL_SCANCODE_UP]) {
        player2.y -= 10;
    }

    else if (player2.y < SCREEN_HEIGHT - 64 && currentKeyStates[SDL_SCANCODE_DOWN]) {
        player2.y += 10;
    }
}

// Function to render graphics
void render() {
    // Clear the renderer
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Set drawing color to white
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // Render the rectangle
    SDL_RenderFillRect(renderer, &player1);
    SDL_RenderFillRect(renderer, &ball);
    SDL_RenderFillRect(renderer, &player2);

    // Present the renderer
    SDL_RenderPresent(renderer);
}

// Function to cap frame rate
void capFrameRate(Uint32 frameStartTime) {

    Uint32 frameTime = SDL_GetTicks() - frameStartTime;
    
    if (frameTime < 1000 / FRAME_RATE) {
        SDL_Delay(1000 / FRAME_RATE - frameTime);
    }
}

#undef main

int main() {

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    window = SDL_CreateWindow("My Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    player1.x = 16;
    player1.y = SCREEN_HEIGHT / 2 - 64;
    player1.w = 16;
    player1.h = 64;

    player2.x = SCREEN_WIDTH - 32;
    player2.y = SCREEN_HEIGHT / 2 - 64;
    player2.w = 16;
    player2.h = 64;

    ball.x = SCREEN_WIDTH / 2 - 32;
    ball.y = SCREEN_HEIGHT / 2 - 32;
    ball.w = 32;
    ball.h = 32;

    while (true) {
        
        Uint32 frameStartTime = SDL_GetTicks();

        handleEvents();
        update();
        render();

        capFrameRate(frameStartTime);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}