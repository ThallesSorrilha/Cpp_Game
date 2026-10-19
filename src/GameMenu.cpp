#include "../include/GameMenu.h"

#include <iostream>

#include "../include/TextureManager.h"
#include "../include/definitions/Definitions.h"
#include "../include/enums/SpriteID.h"

GameMenu::GameMenu(const Config &config)
    : GameScene(config.gameScene),
      options(config.options),
      selectedOption(config.selectedOption),
      panelBackground(TextureManager::load(SpriteID::NinePathPanelBackground))
{
    if (panelBackground == nullptr)
    {
        std::cerr << "GameMenu could not load the start panel background" << std::endl;
    }
}

void GameMenu::handleInput()
{
    const Uint8 *keyStates = SDL_GetKeyboardState(nullptr);
    const bool spaceIsDown = keyStates[SDL_SCANCODE_SPACE] != 0;

    if (spaceIsDown && !spaceWasDown)
    {
        startPressed = true;
    }

    spaceWasDown = spaceIsDown;
}

void GameMenu::update(float deltaTime) { (void)deltaTime; }

void GameMenu::draw()
{
    if (!startPressed || panelBackground == nullptr)
    {
        return;
    }

    int textureWidth = 0;
    int textureHeight = 0;
    if (SDL_QueryTexture(panelBackground, nullptr, nullptr, &textureWidth, &textureHeight) != 0)
    {
        std::cerr << "GameMenu could not query the start panel background: "
                  << SDL_GetError() << std::endl;
        return;
    }

    SDL_Rect destination{
        (SCREEN_WIDTH - textureWidth) / 2,
        (SCREEN_HEIGHT - textureHeight) / 2,
        textureWidth,
        textureHeight};
    TextureManager::drawScreen(panelBackground, destination);
}
