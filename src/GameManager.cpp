#include "../include/GameManager.h"

#include <iostream>
#include <SDL2/SDL_ttf.h>

#include "../include/GameMenu.h"
#include "../include/AudioManager.h"
#include "../include/definitions/Definitions.h"
#include "../include/TextureManager.h"

bool GameManager::init()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        std::cerr << "Video error" << std::endl;
        std::cerr << SDL_GetError() << std::endl;
        return false;
    }

    if (TTF_Init() < 0)
    {
        std::cerr << "SDL_ttf initialization failed" << std::endl;
        std::cerr << TTF_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) == 0)
    {
        std::cerr << "SDL_image initialization failed" << std::endl;
        std::cerr << IMG_GetError() << std::endl;
        TTF_Quit();
        SDL_Quit();
        return false;
    }

    window = SDL_CreateWindow("Jogo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window)
    {
        std::cerr << "Window error" << std::endl;
        std::cerr << SDL_GetError() << std::endl;
        SDL_Quit();
        TTF_Quit();
        IMG_Quit();
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        std::cerr << "Renderer error" << std::endl;
        std::cerr << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        TTF_Quit();
        IMG_Quit();
        window = nullptr;
        return false;
    }

    if (!TextureManager::init(renderer))
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        TTF_Quit();
        IMG_Quit();
        renderer = nullptr;
        window = nullptr;
        return false;
    }

    if (!AudioManager::init())
    {
        std::cerr << "Audio initialization failed, continuing without audio" << std::endl;
    }

    running = true;
    try
    {
        scene = std::make_unique<GameMenu>(GameMenu::Config{});
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Scene creation error: " << ex.what() << std::endl;
        TextureManager::shutdown();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        TTF_Quit();
        IMG_Quit();
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
    scene.reset();
    AudioManager::shutdown();
    TextureManager::shutdown();
    IMG_Quit();
    TTF_Quit();
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

    if (scene)
        scene->handleInput();
}

void GameManager::update(float deltaTime)
{
    if (scene)
        scene->update(deltaTime);
}

void GameManager::draw()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    if (scene)
        scene->draw();

    SDL_RenderPresent(renderer);
}

SDL_Renderer *GameManager::getRenderer()
{
    return GameManager::renderer;
}
