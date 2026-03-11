#include "GameMenu.h"

GameMenu::GameMenu(const Config &config)
    : GameScene(config.gameScene),
      options(config.options),
      selectedOption(config.selectedOption)
{
}

bool GameMenu::init() { return true; }
void GameMenu::handleInput() {}
void GameMenu::update(float deltaTime) {}
void GameMenu::draw() {}
void GameMenu::shutdown() {}
