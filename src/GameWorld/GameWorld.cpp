#include "GameWorld.h"
#include <iostream>
#include "../TextureManager/TextureManager.h"

GameWorld::GameWorld(const Config &config)
    : GameScene(config.gameScene),
      worldId(config.worldId)
{
}

bool GameWorld::init()
{
    player = std::make_unique<Player>(Player::Config{.character = {.dynamicObject = {.gameObject = {.position = {0.0f, 0.0f}, .textureFilePath = "player.png"}}}});
    if (!player->init())
    {
        std::cerr << "Player init failed" << std::endl;
        return false;
    }
    return true;
}

void GameWorld::handleInput()
{
    if (player)
        player->handleInput();
}

void GameWorld::update(float deltaTime)
{
    if (player)
        player->update(deltaTime);
}

void GameWorld::draw()
{
    if (player)
    {
        player->draw();
    }
}

void GameWorld::shutdown()
{
    if (player)
    {
        player->shutdown();
        player.reset();
    }
}
