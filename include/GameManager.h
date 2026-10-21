#pragma once

#include <SDL2/SDL.h>
#include <memory>

#include "GameScene.h"

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

    static inline std::unique_ptr<GameScene> worldLayer = nullptr;
    static inline std::unique_ptr<GameScene> overlayLayer = nullptr;

    static constexpr int FPS = 60;
    static constexpr Uint32 DELAY_TIME = 1000 / FPS;
};