#include "GameWorld.h"
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

    auto player = std::make_unique<Player>(Player::Config{.character = {.dynamicObject = {.gameObject = {.position = {4.0f, 4.0f}, .textureID = TextureID::Player}}}});
    cameraTarget = player.get();
    auto enemy1 = std::make_unique<Enemy>(Enemy::Config{.character = {.dynamicObject = {.gameObject = {.position = {2.0f, 2.0f}, .textureID = TextureID::Enemy}}}});
    auto enemy2 = std::make_unique<Enemy>(Enemy::Config{.character = {.dynamicObject = {.gameObject = {.position = {2.0f, 4.0f}, .textureID = TextureID::Enemy}}}});
    auto enemy3 = std::make_unique<Enemy>(Enemy::Config{.character = {.dynamicObject = {.gameObject = {.position = {4.0f, 2.0f}, .textureID = TextureID::Enemy}}}});

    player->setCollisionMap(tileMap.get());
    enemy1->setCollisionMap(tileMap.get());
    enemy2->setCollisionMap(tileMap.get());
    enemy3->setCollisionMap(tileMap.get());

    gameObjects.push_back(std::move(player));
    gameObjects.push_back(std::move(enemy1));
    gameObjects.push_back(std::move(enemy2));
    gameObjects.push_back(std::move(enemy3));

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
