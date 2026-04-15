#include "GameManager.h"

#include <iostream>

#include "../GameObject/GameObject.h"
#include "../GameWorld/GameWorld.h"
#include "../utils/Definitions.h"
#include "../TextureManager/TextureManager.h"

bool GameManager::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "Video error" << std::endl;
        std::cerr << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("Jogo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window)
    {
        std::cerr << "Window error" << std::endl;
        std::cerr << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        std::cerr << "Renderer error" << std::endl;
        std::cerr << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        window = nullptr;
        return false;
    }

    if (!TextureManager::init(renderer))
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        renderer = nullptr;
        window = nullptr;
        return false;
    }

    running = true;
    try
    {
        world = std::make_unique<GameWorld>(GameWorld::Config{});
    }
    catch (const std::exception &ex)
    {
        std::cerr << "World creation error: " << ex.what() << std::endl;
        TextureManager::shutdown();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        renderer = nullptr;
        window = nullptr;
        return false;
    }

    return true;
}

void GameManager::run()
{
    Uint32 lastTime = SDL_GetTicks();
    while (running)
    {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;
        if (deltaTime > 0.050f)
            deltaTime = 0.050f;

        GameManager::handleInput();
        GameManager::update(deltaTime);
        GameManager::draw();

        Uint32 frameTime = SDL_GetTicks() - currentTime;
        if (frameTime < DELAY_TIME)
        {
            SDL_Delay(static_cast<Uint32>(DELAY_TIME - frameTime));
        }
    }
}

void GameManager::shutdown()
{
    world.reset();
    TextureManager::shutdown();
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

    if (world)
        world->handleInput();
}

void GameManager::update(float deltaTime)
{
    if (world)
        world->update(deltaTime);
}

void GameManager::draw()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    if (world)
        world->draw();

    SDL_RenderPresent(renderer);
}

SDL_Renderer *GameManager::getRenderer()
{
    return GameManager::renderer;
}
