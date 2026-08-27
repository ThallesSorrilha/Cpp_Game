#include "../include/GameWorld.h"

#include <random>
#include <string>

#include "../include/TextureManager.h"
#include "../include/Player.h"
#include "../include/Enemy.h"
#include "../include/Item.h"
#include "../include/AttackObject.h"
#include "../include/definitions/Definitions.h"
#include "../include/enums/SpriteID.h"
#include "../include/enums/MapID.h"
#include "../include/enums/LayerID.h"
#include "../include/ColliderManager.h"
#include "../include/AudioManager.h"

GameWorld::GameWorld(const Config &config)
    : GameScene(config.gameScene)
{
    tileMap = std::make_unique<TileMap>(TileMap::Config{.mapID = MapID::Map06});

    this->audio = Audio::Music_Village;

    auto player = std::make_unique<Player>(Player::Config{.character = {.dynamicObject = {.physicalObject = {.gameObject = {.position = {4.0f, 4.0f}, .size = {1.0f, 1.0f}, .spriteID = SpriteID::Player}, .colliderBox = {.offset = {0.20f, 0.20f}, .size = {0.60f, 0.60f}}}}}});
    cameraTarget = player.get();
    this->player = player.get();
    player->setCollisionMap(tileMap.get());
    physicalObjects.push_back(std::move(player));

    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> spawnXDist(1.0f, tileMap->getWidthInBlocks() - 2.0f);
    std::uniform_real_distribution<float> spawnYDist(1.0f, tileMap->getHeightInBlocks() - 2.0f);

    for (int i = 0; i < 20; ++i)
    {
        auto extraEnemy = std::make_unique<Enemy>(Enemy::Config{.character = {.dynamicObject = {.physicalObject = {.gameObject = {.position = {spawnXDist(rng), spawnYDist(rng)}, .spriteID = SpriteID::Enemy}, .colliderBox = {.offset = {0.20f, 0.20f}, .size = {0.60f, 0.60f}}}}}});
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

    AudioManager::playMusic(this->audio, -1);
}

GameWorld::~GameWorld()
{
    if (scoreTexture != nullptr)
    {
        SDL_DestroyTexture(scoreTexture);
    }
    if (scoreFont != nullptr)
    {
        TTF_CloseFont(scoreFont);
    }

    player = nullptr;
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
    processEnemyDrops();

    ColliderManager::detectObjectCollisions(physicalObjects);

    if (cameraTarget)
    {
        Camera::follow(cameraTarget->getPosition(), cameraTarget->getSize());
        TextureManager::setCameraPosition(Camera::getPosition());
    }

    killObjects();
}

void GameWorld::processEnemyDrops()
{
    for (auto &obj : physicalObjects)
    {
        auto *enemy = dynamic_cast<Enemy *>(obj.get());
        if (!enemy || !enemy->hasCoinToDrop())
        {
            continue;
        }

        auto coin = std::make_unique<Item>(Item::Config{.staticObject = {.physicalObject = {.gameObject = {.position = enemy->getPosition(), .size = {0.5f, 0.5f}, .spriteID = SpriteID::Coin}, .colliderBox = {.offset = {0.0f, 0.0f}, .size = {0.5f, 0.5f}}}}});
        enemy->markCoinAsDropped();
        physicalObjects.push_back(std::move(coin));
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

void GameWorld::drawScore(SDL_Renderer *renderer)
{
    if (renderer == nullptr || player == nullptr)
    {
        return;
    }

    if (scoreFont == nullptr)
    {
        scoreFont = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", 24);
        if (scoreFont == nullptr)
        {
            return;
        }
    }

    const int coins = player->getCoins();
    if (coins != renderedCoins)
    {
        if (scoreTexture != nullptr)
        {
            SDL_DestroyTexture(scoreTexture);
            scoreTexture = nullptr;
        }

        const std::string scoreText = "PONTOS: " + std::to_string(coins) + " /10";
        SDL_Color textColor{255, 255, 255, 255};
        SDL_Surface *surface = TTF_RenderUTF8_Blended(scoreFont, scoreText.c_str(), textColor);
        if (surface == nullptr)
        {
            return;
        }

        scoreTexture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        if (scoreTexture == nullptr)
        {
            return;
        }

        renderedCoins = coins;
    }

    int width = 0;
    int height = 0;
    SDL_QueryTexture(scoreTexture, nullptr, nullptr, &width, &height);
    SDL_Rect destination{16, 16, width, height};
    SDL_RenderCopy(renderer, scoreTexture, nullptr, &destination);
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
        if (!(*it) || !(*it)->isExist())
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
