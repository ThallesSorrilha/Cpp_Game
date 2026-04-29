#include "../include/GameWorld.h"

#include <random>

#include "../include/TextureManager.h"
#include "../include/Player.h"
#include "../include/Enemy.h"
#include "../include/AttackObject.h"
#include "../include/definitions/Definitions.h"
#include "../include/enums/SpriteID.h"
#include "../include/enums/MapID.h"
#include "../include/enums/LayerID.h"
#include "../include/ColliderManager.h"

GameWorld::GameWorld(const Config &config)
    : GameScene(config.gameScene)
{
    tileMap = std::make_unique<TileMap>(TileMap::Config{.mapID = MapID::Map03});

    auto player = std::make_unique<Player>(Player::Config{.character = {.dynamicObject = {.physicalObject = {.gameObject = {.position = {4.0f, 4.0f}, .size = {1.0f, 1.0f}, .spriteID = SpriteID::Player}, .colliderBox = {.offset = {0.02f, 0.02f}, .size = {0.96f, 0.96f}}}}}});
    cameraTarget = player.get();
    player->setCollisionMap(tileMap.get());
    physicalObjects.push_back(std::move(player));

    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> spawnXDist(1.0f, tileMap->getWidthInBlocks() - 2.0f);
    std::uniform_real_distribution<float> spawnYDist(1.0f, tileMap->getHeightInBlocks() - 2.0f);

    for (int i = 0; i < 19; ++i)
    {
        auto extraEnemy = std::make_unique<Enemy>(Enemy::Config{.character = {.dynamicObject = {.physicalObject = {.gameObject = {.position = {spawnXDist(rng), spawnYDist(rng)}, .spriteID = SpriteID::Enemy}}}}});
        extraEnemy->setCollisionMap(tileMap.get());
        physicalObjects.push_back(std::move(extraEnemy));
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

    physicalObjects.clear();
    tileMap.reset();
}

void GameWorld::handleInput()
{
    for (auto &obj : physicalObjects)
    {
        if (obj)
            obj->handleInput();
    }
}

void GameWorld::update(float deltaTime)
{
    tileMap->update(deltaTime);

    for (auto &obj : physicalObjects)
    {
        if (obj)
        {
            obj->update(deltaTime);
        }
    }

    processPendingAttackRequests();

    ColliderManager::detectObjectCollisions(physicalObjects);

    if (cameraTarget)
    {
        Camera::follow(cameraTarget->getPosition(), cameraTarget->getSize());
        TextureManager::setCameraPosition(Camera::getPosition());
    }

    killObjects();
}

void GameWorld::draw()
{
    tileMap->draw();

    for (auto &obj : physicalObjects)
    {
        if (obj)
            obj->draw();
    }
}

void GameWorld::processPendingAttackRequests()
{
    for (auto &obj : physicalObjects)
    {
        if (!obj)
        {
            continue;
        }

        if (!obj->hasObjToCreate())
        {
            continue;
        }
        
        auto *character = dynamic_cast<Character *>(obj.get());
        if (!character)
        {
            continue;
        }

        auto attackObject = character->createAttack();
        if (attackObject == nullptr)
        {
            continue;
        }

        attackObject->setCollisionMap(tileMap.get());
        physicalObjects.push_back(std::move(attackObject));
    }
}

void GameWorld::killObjects()
{
    for (auto it = physicalObjects.begin(); it != physicalObjects.end();)
    {
        if (!(*it) || !(*it)->isAlive())
        {
            if (cameraTarget == it->get())
            {
                cameraTarget = nullptr;
            }
            it = physicalObjects.erase(it);
            continue;
        }

        ++it;
    }
}
