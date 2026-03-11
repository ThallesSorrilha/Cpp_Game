#include "GameWorld.h"
#include <iostream>

GameWorld::GameWorld(const Config &config)
    : GameScene(config.gameScene),
    wordlId(config.wordlId)
{
}

bool GameWorld::init() { return true; }
void GameWorld::handleInput() {}
void GameWorld::update(float deltaTime) {}
void GameWorld::draw() {}
void GameWorld::shutdown(){}
