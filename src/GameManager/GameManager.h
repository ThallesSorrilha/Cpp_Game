#pragma once
#include <SDL2/SDL.h>
#include <chrono>

class GameManager
{
public:
    bool init();
    void run();
    void shutdown();

private:
    void handleInput();
    void update();
    void draw();

    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    bool running = false;

    const int FPS = 60;
    const Uint32 DELAY_TIME = 1000.0f / FPS;
};