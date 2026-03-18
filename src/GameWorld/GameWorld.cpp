#include "GameWorld.h"
#include <iostream>
#include "../TextureManager/TextureManager.h"
#include "../Player/Player.h"
#include "../Enemy/Enemy.h"

GameWorld::GameWorld(const Config &config)
    : GameScene(config.gameScene),
      worldId(config.worldId)
{
}

bool GameWorld::init()
{
    auto player = std::make_unique<Player>(Player::Config{.character = {.dynamicObject = {.gameObject = {.position = {3.0f, 3.0f}, .textureFilePath = "player.png"}}}});
    auto enemy1 = std::make_unique<Enemy>(Enemy::Config{.character = {.dynamicObject = {.gameObject = {.position = {0.0f, 0.0f}, .textureFilePath = "player.png"}}}});
    auto enemy2 = std::make_unique<Enemy>(Enemy::Config{.character = {.dynamicObject = {.gameObject = {.position = {1.0f, 1.0f}, .textureFilePath = "player.png"}}}});

    gameObjects.push_back(std::move(player));
    gameObjects.push_back(std::move(enemy1));
    gameObjects.push_back(std::move(enemy2));

    for (auto &obj : gameObjects)
    {
        if (obj)
        {
            if (!obj->init())
            {
                std::cerr << "Obj init failed" << std::endl;
                return false;
            }
        }
    }

    return true;
}

void GameWorld::handleInput()
{
    for (auto& obj : gameObjects)
    {
        if (obj)
            obj->handleInput();
    }
}

void GameWorld::update(float deltaTime)
{
    for (auto& obj : gameObjects)
    {
        if (obj)
            obj->update(deltaTime);
    }
}

void GameWorld::draw()
{
    for (auto& obj : gameObjects)
    {
        if (obj)
            obj->draw();
    }
}

void GameWorld::shutdown()
{
    for (auto& obj : gameObjects)
    {
        if (obj)
        {
            obj->shutdown();
        }
    }

    gameObjects.clear();
}
