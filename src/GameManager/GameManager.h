#pragma once
#include <SDL2/SDL.h>

class GameManager
{
public:
    bool init();
    void run();
    void shutdown();

private:
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    bool running = false;
};