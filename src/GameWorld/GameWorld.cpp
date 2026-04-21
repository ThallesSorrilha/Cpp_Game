#include "GameWorld.h"

#include <random>

#include "../TextureManager/TextureManager.h"
#include "../Player/Player.h"
#include "../Enemy/Enemy.h"
#include "../AttackObject/AttackObject.h"
#include "../definitions/Definitions.h"
#include "../enums/SpriteID.h"
#include "../enums/MapID.h"
#include "../enums/LayerID.h"
#include "../ColliderManager/ColliderManager.h"

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
    constexpr Vector2D kAttackSize = {1.0f, 1.0f};
    constexpr float kSpawnDistance = 1.0f;

    std::list<std::unique_ptr<PhysicalObject>> objectsToSpawn;

    for (auto &obj : physicalObjects)
    {
        if (!obj)
        {
            continue;
        }

        auto *character = dynamic_cast<Character *>(obj.get());
        if (!character)
        {
            continue;
        }

        AttackRequest request;
        while (character->consumeAttackRequest(request))
        {
            Vector2D direction = request.direction;
            if (direction.x == 0.0f && direction.y == 0.0f)
            {
                direction = {0.0f, 1.0f};
            }
            else
            {
                direction.normalize();
            }

            const Vector2D playerCenter = {
                request.position.x + obj->getSize().x * 0.5f,
                request.position.y + obj->getSize().y * 0.5f};

            const Vector2D spawnCenter = {
                playerCenter.x + direction.x * kSpawnDistance,
                playerCenter.y + direction.y * kSpawnDistance};

            const Vector2D spawnTopLeft = {
                spawnCenter.x - kAttackSize.x * 0.5f,
                spawnCenter.y - kAttackSize.y * 0.5f};

            auto attackObject = std::make_unique<AttackObject>(AttackObject::Config{
                .dynamicObject = {
                    .physicalObject = {
                        .gameObject = {
                            .position = spawnTopLeft,
                            .size = kAttackSize,
                            .spriteID = SpriteID::Attack},
                        .colliderBox = {
                            .collisionLayer = request.collisionLayer,
                            .collisionMask = request.collisionMask,
                            .offset = {0.0f, 0.0f},
                            .size = kAttackSize}}},
                .attackDamage = request.damage,
                .isAttacking = true});

            attackObject->setCollisionMap(tileMap.get());
            objectsToSpawn.push_back(std::move(attackObject));
        }
    }

    for (auto &spawnedObject : objectsToSpawn)
    {
        physicalObjects.push_back(std::move(spawnedObject));
    }
}
