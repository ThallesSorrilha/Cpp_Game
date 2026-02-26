#include "GameManager.h"
#include <iostream>

Uint32 frameStart;
Uint32 frameTime;

bool GameManager::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "Video failed\n";
        std::cout << SDL_GetError() << "\n";
        return false;
    }
    window = SDL_CreateWindow("Jogo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window)
    {
        std::cerr << "Window failed\n";
        std::cout << SDL_GetError() << "\n";
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    running = true;
    return true;
}

void GameManager::run()
{
    while (running)
    {
        frameStart = SDL_GetTicks();
        GameManager::handleInput();
        GameManager::update();
        GameManager::draw();
        frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < DELAY_TIME)
        {
            SDL_Delay((int)(DELAY_TIME - frameTime));
        }
    }
}

void GameManager::shutdown()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void GameManager::handleInput()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
            running = false;
    }
}

void GameManager::update()
{
}

void GameManager::draw()
{
    SDL_SetRenderDrawColor(renderer, 0, 100, 255, 255);
    SDL_RenderClear(renderer);
    SDL_Rect r{350, 250, 100, 100};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &r);
    SDL_RenderPresent(renderer);
}