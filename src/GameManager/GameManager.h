#pragma once
#include <SDL2/SDL.h>
#include "../GameWorld/GameWorld.h"

class GameManager
{
public:
    static bool init();
    static void run();
    static void shutdown();
    static SDL_Renderer* getRenderer();

private:
    static void handleInput();
    static void update(float deltaTime);
    static void draw();

    static inline SDL_Window *window = nullptr;
    static inline SDL_Renderer *renderer = nullptr;
    static inline bool running = false;

    static inline std::unique_ptr<GameWorld> world = nullptr;

    static const int FPS = 60;
    static const Uint32 DELAY_TIME = 1000 / FPS;
};