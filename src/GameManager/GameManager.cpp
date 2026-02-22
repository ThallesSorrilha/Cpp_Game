#include "GameManager.h"
#include <iostream>

bool GameManager::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "Video failed\n";
        return false;
    }
    window = SDL_CreateWindow("Jogo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window)
    {
        std::cerr << "Window failed\n";
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    running = true;
    return true;
}

void GameManager::run()
{
    SDL_Event e;
    while (running)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                running = false;
        }
        SDL_SetRenderDrawColor(renderer, 0, 100, 255, 255);
        SDL_RenderClear(renderer);
        SDL_Rect r{350, 250, 100, 100};
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &r);
        SDL_RenderPresent(renderer);
    }
}

void GameManager::shutdown()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}