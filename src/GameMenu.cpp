#include "../include/GameMenu.h"

GameMenu::GameMenu(const Config &config)
    : GameScene(config.gameScene),
      options(config.options),
      selectedOption(config.selectedOption)
{
}

void GameMenu::handleInput() {}
void GameMenu::update(float deltaTime) { (void)deltaTime; }
void GameMenu::draw() {}
