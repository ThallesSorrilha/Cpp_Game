#include "GameWorld.h"
#include <iostream>
#include "../TextureManager/TextureManager.h"
#include "../Player/Player.h"
#include "../Enemy/Enemy.h"

GameWorld::GameWorld(const Config &config)
    : GameScene(config.gameScene)
{
}

bool GameWorld::init()
{
    tileMap = std::make_unique<TileMap>(TileMap::Config{.tmxFilePath = "assets/maps/map01.tmx"});
    if (!tileMap->init())
    {
        std::cerr << "TileMap init failed" << std::endl;
        return false;
    }

    auto player = std::make_unique<Player>(Player::Config{.character = {.dynamicObject = {.gameObject = {.position = {5.0f, 5.0f}, .textureFilePath = "assets/sprites/player.png"}}}});
    auto enemy1 = std::make_unique<Enemy>(Enemy::Config{.character = {.dynamicObject = {.gameObject = {.position = {2.0f, 2.0f}, .textureFilePath = "assets/sprites/enemy.png"}}}});
    auto enemy2 = std::make_unique<Enemy>(Enemy::Config{.character = {.dynamicObject = {.gameObject = {.position = {3.0f, 3.0f}, .textureFilePath = "assets/sprites/enemy.png"}}}});
    auto enemy3 = std::make_unique<Enemy>(Enemy::Config{.character = {.dynamicObject = {.gameObject = {.position = {4.0f, 4.0f}, .textureFilePath = "assets/sprites/enemy.png"}}}});

    gameObjects.push_back(std::move(player));
    gameObjects.push_back(std::move(enemy1));
    gameObjects.push_back(std::move(enemy2));
    gameObjects.push_back(std::move(enemy3));

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
    for (auto &obj : gameObjects)
    {
        if (obj)
            obj->handleInput();
    }
}

void GameWorld::update(float deltaTime)
{
    if (tileMap)
    {
        tileMap->update(deltaTime);
    }

    for (auto &obj : gameObjects)
    {
        if (obj)
            obj->update(deltaTime);
    }
}

void GameWorld::draw()
{
    if (tileMap)
    {
        tileMap->draw();
    }

    for (auto &obj : gameObjects)
    {
        if (obj)
            obj->draw();
    }
}

void GameWorld::shutdown()
{
    if (tileMap)
    {
        tileMap->shutdown();
        tileMap.reset();
    }

    for (auto &obj : gameObjects)
    {
        if (obj)
        {
            obj->shutdown();
        }
    }

    gameObjects.clear();
}

