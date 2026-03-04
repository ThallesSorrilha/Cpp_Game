#pragma once
#include <SDL2/SDL.h>

class GameManager
{
public:
    bool init();
    void run();
    void shutdown();

private:
    void handleInput();
    void update(float deltaTime);
    void draw();

    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    bool running = false;

    static const int FPS = 60;
    static const Uint32 DELAY_TIME = 1000 / FPS;
};