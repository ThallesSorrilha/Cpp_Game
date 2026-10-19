#include "../include/GameScene.h"

#include <SDL2/SDL.h>

GameScene::GameScene(const Config &config)
    : initialized(config.initialized),
      paused(config.paused)
{
}

void GameScene::synchronizeInputState()
{
    const Uint8 *keyStates = SDL_GetKeyboardState(nullptr);
    spaceWasDown = keyStates[SDL_SCANCODE_SPACE] != 0;
}