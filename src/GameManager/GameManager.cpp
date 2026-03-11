#include "GameManager.h"
#include <iostream>
#include "../Player/Player.h"
#include "../GameObject/GameObject.h"

bool GameManager::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "Video failed" << std::endl;
        std::cout << SDL_GetError() << std::endl;
        return false;
    }
    window = SDL_CreateWindow("Jogo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window)
    {
        std::cerr << "Window failed" << std::endl;
        std::cout << SDL_GetError() << std::endl;
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        std::cerr << "Renderer failed" << std::endl;
        std::cout << SDL_GetError() << std::endl;
        return false;
    }
    running = true;

    return true;
}

void GameManager::run()
{
    Uint32 lastTime = SDL_GetTicks();

    Player player(Player::Config{.character = {.dynamicObject = {.gameObject = {.position = {0.0f, 0.0f}}}}});

    while (running)
    {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;
        if (deltaTime > 0.050f)
        {
            deltaTime = 0.050f;
        }
        GameManager::handleInput();
        GameManager::update(deltaTime);
        GameManager::draw();

        Uint32 frameTime = SDL_GetTicks() - currentTime;
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
        {
            running = false;
        }
    }
}

void GameManager::update(float deltaTime)
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