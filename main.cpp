#include <iostream>
#include "inc/SDL.h"

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Rect player1;
SDL_Rect player2;
SDL_Rect ball;

int playerSpeed = 800;
int ballVelocityX = 400;
int ballVelocityY = 400;

const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 544;

// Desired frame rate (frames per second)
// const int FRAME_RATE = 60; 

// Function to handle events
void handleEvents() {

    SDL_Event event;

    while (SDL_PollEvent(&event)) {

        if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE) {
            
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
            exit(0);
        }
    }
}

bool hasCollision(SDL_Rect player, SDL_Rect ball) {

    return player.x < ball.x + ball.w && player.x + player.w > ball.x &&
            player.y < ball.y + ball.h && player.y + player.h > ball.y;
} 

// Function to update rectangle movement.
void update(float deltaTime) {

    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

    if (player1.y > 0 && currentKeyStates[SDL_SCANCODE_W]) {
        player1.y -= playerSpeed * deltaTime;
    }

    else if (player1.y < SCREEN_HEIGHT - player1.h && currentKeyStates[SDL_SCANCODE_S]) {
        player1.y += playerSpeed * deltaTime;
    }

    if (player2.y > 0 && currentKeyStates[SDL_SCANCODE_UP]) {
        player2.y -= playerSpeed * deltaTime;
    }

    if (player2.y < SCREEN_HEIGHT - player2.h && currentKeyStates[SDL_SCANCODE_DOWN]) {
        player2.y += playerSpeed * deltaTime;
    }

    if (ball.x > SCREEN_WIDTH + ball.w || ball.x < -ball.w)
    {
        ball.x = SCREEN_WIDTH / 2 - ball.w;
        ball.y = SCREEN_HEIGHT / 2 - ball.h;

        ballVelocityX *= -1;
        ballVelocityY *= -1;
    }

    if (ball.y < 0 || ball.y > SCREEN_HEIGHT - ball.h)
    {
        ballVelocityY *= -1;
    }

    if (hasCollision(player1, ball) || hasCollision(player2, ball))
    {
        ballVelocityX *= -1;
    }
    
    ball.x += ballVelocityX * deltaTime;
    ball.y += ballVelocityY * deltaTime;
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

    SDL_RenderDrawLine(renderer, SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT);

    SDL_RenderFillRect(renderer, &ball);
    SDL_RenderFillRect(renderer, &player2);

    // Present the renderer
    SDL_RenderPresent(renderer);
}

// Function to cap frame rate
// void capFrameRate(Uint32 frameStartTime) {

//     Uint32 frameTime = SDL_GetTicks() - frameStartTime;
    
//     if (frameTime < 1000 / FRAME_RATE) {
//         SDL_Delay(1000 / FRAME_RATE - frameTime);
//     }
// }

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
    
    player1 = {16, SCREEN_HEIGHT / 2 - 64, 16, 74};
    player2 = {SCREEN_WIDTH - 32, SCREEN_HEIGHT / 2 - 64, 16, 74};

    ball = {SCREEN_WIDTH / 2 - 26, SCREEN_HEIGHT / 2 - 26, 26, 26};

    Uint32 previousFrameTime = SDL_GetTicks();
    Uint32 currentFrameTime = previousFrameTime;
    float deltaTime = 0.0f;

    while (true) {
        
        currentFrameTime = SDL_GetTicks();

    // Calculate delta time in seconds
        deltaTime = (currentFrameTime - previousFrameTime) / 1000.0f; // Convert to seconds

    // Update the previous frame time for the next iteration
        previousFrameTime = currentFrameTime;

        handleEvents();
        update(deltaTime);
        render();

        // capFrameRate(currentFrameTime);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}