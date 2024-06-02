#include <iostream>
#include "inc/SDL.h"

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 544;

SDL_Rect player1 = {16, SCREEN_HEIGHT / 2 - 64, 16, 74};
SDL_Rect player2 = {SCREEN_WIDTH - 32, SCREEN_HEIGHT / 2 - 64, 16, 74};
SDL_Rect ball = {SCREEN_WIDTH / 2 - 26, SCREEN_HEIGHT / 2 - 26, 26, 26};

int playerSpeed = 800;
int ballVelocityX = 400;
int ballVelocityY = 400;

bool isAutoPlayMode = true;

void quitGame() {

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void handleEvents() {

    SDL_Event event;

    while (SDL_PollEvent(&event)) {

        if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE) {
            
            quitGame();
            exit(0);
        }
    }
}

bool hasCollision(SDL_Rect player, SDL_Rect ball) {

    return player.x < ball.x + ball.w && player.x + player.w > ball.x &&
            player.y < ball.y + ball.h && player.y + player.h > ball.y;
} 

void update(float deltaTime) {

    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

    if (player1.y > 0 && currentKeyStates[SDL_SCANCODE_W]) {
        player1.y -= playerSpeed * deltaTime;
    }

    else if (player1.y < SCREEN_HEIGHT - player1.h && currentKeyStates[SDL_SCANCODE_S]) {
        player1.y += playerSpeed * deltaTime;
    }

    if (currentKeyStates[SDL_SCANCODE_D]) {
        isAutoPlayMode = !isAutoPlayMode;
    }

    if (isAutoPlayMode && ball.y < SCREEN_HEIGHT - player2.h) {
        player2.y = ball.y;
    }

    if (player2.y > 0 && currentKeyStates[SDL_SCANCODE_UP]) {
        player2.y -= playerSpeed * deltaTime;
    }

    if (player2.y < SCREEN_HEIGHT - player2.h && currentKeyStates[SDL_SCANCODE_DOWN]) {
        player2.y += playerSpeed * deltaTime;
    }

    if (ball.x > SCREEN_WIDTH + ball.w || ball.x < -ball.w) {

        ball.x = SCREEN_WIDTH / 2 - ball.w;
        ball.y = SCREEN_HEIGHT / 2 - ball.h;

        ballVelocityX *= -1;
        ballVelocityY *= -1;
    }

    if (ball.y < 0 || ball.y > SCREEN_HEIGHT - ball.h) {
        ballVelocityY *= -1;
    }

    if (hasCollision(player1, ball) || hasCollision(player2, ball)) {
        ballVelocityX *= -1;
    }
    
    ball.x += ballVelocityX * deltaTime;
    ball.y += ballVelocityY * deltaTime;
}

void render() {
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_RenderFillRect(renderer, &player1);

    SDL_RenderDrawLine(renderer, SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT);

    SDL_RenderFillRect(renderer, &ball);
    SDL_RenderFillRect(renderer, &player2);

    SDL_RenderPresent(renderer);
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
    
    Uint32 previousFrameTime = SDL_GetTicks();
    Uint32 currentFrameTime = previousFrameTime;
    float deltaTime = 0.0f;

    while (true) {
        
        currentFrameTime = SDL_GetTicks();
        deltaTime = (currentFrameTime - previousFrameTime) / 1000.0f;
        previousFrameTime = currentFrameTime;

        handleEvents();
        update(deltaTime);
        render();
    }

    quitGame();
}