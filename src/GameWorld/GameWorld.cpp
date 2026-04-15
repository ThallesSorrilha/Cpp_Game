#include "GameWorld.h"

#include <random>

#include "../TextureManager/TextureManager.h"
#include "../Player/Player.h"
#include "../Enemy/Enemy.h"
#include "../utils/Definitions.h"
#include "../enums/TextureID.h"
#include "../enums/MapID.h"

GameWorld::GameWorld(const Config &config)
    : GameScene(config.gameScene)
{
    tileMap = std::make_unique<TileMap>(TileMap::Config{.mapID = MapID::Map03});

    auto player = std::make_unique<Player>(Player::Config{.character = {.dynamicObject = {.gameObject = {.position = {16.0f, 16.0f}, .size={1.0f, 1.0f}, .textureID = TextureID::Player}}}});
    cameraTarget = player.get();
    player->setCollisionMap(tileMap.get());
    gameObjects.push_back(std::move(player));

    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> spawnXDist(1.0f, tileMap->getWidthInBlocks() - 2.0f);
    std::uniform_real_distribution<float> spawnYDist(1.0f, tileMap->getHeightInBlocks() - 2.0f);

    for (int i = 0; i < 19; ++i)
    {
        auto extraEnemy = std::make_unique<Enemy>(Enemy::Config{.character = {.dynamicObject = {.gameObject = {.position = {spawnXDist(rng), spawnYDist(rng)}, .textureID = TextureID::Enemy}}}});
        extraEnemy->setCollisionMap(tileMap.get());
        gameObjects.push_back(std::move(extraEnemy));
    }

    Camera::init(
        {static_cast<float>(SCREEN_WIDTH) / static_cast<float>(PIXELS_PER_BLOCK),
         static_cast<float>(SCREEN_HEIGHT) / static_cast<float>(PIXELS_PER_BLOCK)},
        {tileMap->getWidthInBlocks(), tileMap->getHeightInBlocks()});

    if (cameraTarget)
    {
        Camera::follow(cameraTarget->getPosition(), cameraTarget->getSize());
        TextureManager::setCameraPosition(Camera::getPosition());
    }
}

GameWorld::~GameWorld()
{
    cameraTarget = nullptr;
    TextureManager::clearCamera();

    gameObjects.clear();
    tileMap.reset();
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
    tileMap->update(deltaTime);

    for (auto &obj : gameObjects)
    {
        if (obj)
        {
            obj->update(deltaTime);
        }
    }

    if (cameraTarget)
    {
        Camera::follow(cameraTarget->getPosition(), cameraTarget->getSize());
        TextureManager::setCameraPosition(Camera::getPosition());
    }
}

void GameWorld::draw()
{
    tileMap->draw();

    for (auto &obj : gameObjects)
    {
        if (obj)
            obj->draw();
    }
}
