#include "../include/GameWorld.h"

#include <algorithm>
#include <random>
#include <string>
#include <utility>

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
#include "../include/definitions/SpriteCutArray.h"
#include "../include/enums/SpriteCutID.h"

namespace
{
constexpr int HUD_SCALE = PIXELS_PER_BLOCK / PIXELS_PER_TILE;
constexpr int HUD_MARGIN = 16;
constexpr int HUD_COIN_GAP = 8;
constexpr int HP_PER_HEART = 2;
constexpr SDL_Color HUD_TINT{255, 255, 255, 255};

SDL_Rect cutRect(SpriteCutID id)
{
    const int index = std::to_underlying(id);
    return SDL_Rect{SpriteCutArray[index][0], SpriteCutArray[index][1], SpriteCutArray[index][2], SpriteCutArray[index][3]};
}
} // namespace

GameWorld::GameWorld(const Config &config)
    : GameScene(config.gameScene)
{
    tileMap = std::make_unique<TileMap>(TileMap::Config{.mapID = MapID::Map07});

    this->audio = Audio::Music_Village;

    auto player = std::make_unique<Player>(Player::Config{.character = {.dynamicObject = {.physicalObject = {.gameObject = {.position = {11.0f, 8.0f}, .size = {1.0f, 1.0f}, .spriteID = SpriteID::Player}, .colliderBox = {.offset = {0.20f, 0.20f}, .size = {0.60f, 0.60f}}}}}});
    cameraTarget = player.get();
    this->player = player.get();
    player->setCollisionMap(tileMap.get());
    physicalObjects.push_back(std::move(player));

    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> spawnXDist(1.0f, tileMap->getWidthInBlocks() - 2.0f);
    std::uniform_real_distribution<float> spawnYDist(1.0f, tileMap->getHeightInBlocks() - 2.0f);

    auto enemy1 = std::make_unique<Enemy>(Enemy::Config{.character = {.dynamicObject = {.physicalObject = {.gameObject = {.position = {28.0f, 6.0f}, .spriteID = SpriteID::Enemy_Skeleton}, .colliderBox = {.offset = {0.20f, 0.20f}, .size = {0.60f, 0.60f}}}}}});
    enemy1->setCollisionMap(tileMap.get());
    physicalObjects.push_back(std::move(enemy1));

    auto enemy2 = std::make_unique<Enemy>(Enemy::Config{.character = {.dynamicObject = {.physicalObject = {.gameObject = {.position = {33.0f, 15.0f}, .spriteID = SpriteID::Enemy_Skeleton}, .colliderBox = {.offset = {0.20f, 0.20f}, .size = {0.60f, 0.60f}}}}}});
    enemy2->setCollisionMap(tileMap.get());
    physicalObjects.push_back(std::move(enemy2));

    auto enemy3 = std::make_unique<Enemy>(Enemy::Config{.character = {.dynamicObject = {.physicalObject = {.gameObject = {.position = {42.0f, 10.0f}, .spriteID = SpriteID::Enemy_Skeleton}, .colliderBox = {.offset = {0.20f, 0.20f}, .size = {0.60f, 0.60f}}}}}});
    enemy3->setCollisionMap(tileMap.get());
    physicalObjects.push_back(std::move(enemy3));

    auto enemy4 = std::make_unique<Enemy>(Enemy::Config{.character = {.dynamicObject = {.physicalObject = {.gameObject = {.position = {52.0f, 6.0f}, .spriteID = SpriteID::Enemy_Skeleton}, .colliderBox = {.offset = {0.20f, 0.20f}, .size = {0.60f, 0.60f}}}}}});
    enemy4->setCollisionMap(tileMap.get());
    physicalObjects.push_back(std::move(enemy4));

    auto enemy5 = std::make_unique<Enemy>(Enemy::Config{.character = {.dynamicObject = {.physicalObject = {.gameObject = {.position = {63.0f, 11.0f}, .spriteID = SpriteID::Enemy_Overcoat}, .colliderBox = {.offset = {0.20f, 0.20f}, .size = {0.60f, 0.60f}}}}}});
    enemy5->setCollisionMap(tileMap.get());
    physicalObjects.push_back(std::move(enemy5));

    auto enemy6 = std::make_unique<Enemy>(Enemy::Config{.character = {.dynamicObject = {.physicalObject = {.gameObject = {.position = {64.0f, 5.0f}, .spriteID = SpriteID::Enemy_Overcoat}, .colliderBox = {.offset = {0.20f, 0.20f}, .size = {0.60f, 0.60f}}}}}});
    enemy6->setCollisionMap(tileMap.get());
    physicalObjects.push_back(std::move(enemy6));

    auto enemy7 = std::make_unique<Enemy>(Enemy::Config{.character = {.dynamicObject = {.physicalObject = {.gameObject = {.position = {73.0f, 17.0f}, .spriteID = SpriteID::Enemy_Overcoat}, .colliderBox = {.offset = {0.20f, 0.20f}, .size = {0.60f, 0.60f}}}}}});
    enemy7->setCollisionMap(tileMap.get());
    physicalObjects.push_back(std::move(enemy7));

    auto enemy8 = std::make_unique<Enemy>(Enemy::Config{.character = {.dynamicObject = {.physicalObject = {.gameObject = {.position = {64.0f, 1.0f}, .spriteID = SpriteID::Enemy_Overcoat}, .colliderBox = {.offset = {0.20f, 0.20f}, .size = {0.60f, 0.60f}}}}}});
    enemy8->setCollisionMap(tileMap.get());
    physicalObjects.push_back(std::move(enemy8));

    auto enemy9 = std::make_unique<Enemy>(Enemy::Config{.character = {.dynamicObject = {.physicalObject = {.gameObject = {.position = {75.0f, 11.0f}, .spriteID = SpriteID::Enemy_Overcoat}, .colliderBox = {.offset = {0.20f, 0.20f}, .size = {0.60f, 0.60f}}}}}});
    enemy9->setCollisionMap(tileMap.get());
    physicalObjects.push_back(std::move(enemy9));

    auto enemy10 = std::make_unique<Enemy>(Enemy::Config{.character = {.dynamicObject = {.physicalObject = {.gameObject = {.position = {80.0f, 10.0f}, .spriteID = SpriteID::Enemy_Overcoat}, .colliderBox = {.offset = {0.20f, 0.20f}, .size = {0.60f, 0.60f}}}}}});
    enemy10->setCollisionMap(tileMap.get());
    physicalObjects.push_back(std::move(enemy10));

    auto enemy11 = std::make_unique<Enemy>(Enemy::Config{.character = {.dynamicObject = {.physicalObject = {.gameObject = {.position = {74.0f, 1.0f}, .spriteID = SpriteID::Enemy_Overcoat}, .colliderBox = {.offset = {0.20f, 0.20f}, .size = {0.60f, 0.60f}}}}}});
    enemy11->setCollisionMap(tileMap.get());
    physicalObjects.push_back(std::move(enemy11));

    auto enemy12 = std::make_unique<Enemy>(Enemy::Config{.character = {.dynamicObject = {.physicalObject = {.gameObject = {.position = {53.0f, 15.0f}, .spriteID = SpriteID::Enemy_Skeleton}, .colliderBox = {.offset = {0.20f, 0.20f}, .size = {0.60f, 0.60f}}}}}});
    enemy12->setCollisionMap(tileMap.get());
    physicalObjects.push_back(std::move(enemy12));

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
    const Uint8 *keyStates = SDL_GetKeyboardState(nullptr);
    const bool spaceIsDown = keyStates[SDL_SCANCODE_SPACE] != 0;
    if (spaceIsDown && !spaceWasDown)
    {
        transitionRequested = true;
    }
    spaceWasDown = spaceIsDown;

    for (auto &obj : physicalObjects)
    {
        if (obj)
            obj->handleInput();
    }
}

GameScene::Transition GameWorld::requestedTransition() const
{
    return transitionRequested ? Transition::Menu : Transition::None;
}

void GameWorld::clearTransitionRequest()
{
    transitionRequested = false;
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

void GameWorld::drawUi(SDL_Renderer *renderer)
{
    if (renderer == nullptr || player == nullptr)
    {
        return;
    }

    drawHearts();
    drawCoins();
}

void GameWorld::drawHearts() const
{
    SDL_Texture *heartTexture = TextureManager::load(SpriteID::UiHeart);
    if (heartTexture == nullptr || player == nullptr)
    {
        return;
    }

    const int heartCount = player->getMaxHp() / HP_PER_HEART;
    const int currentHp = player->getCurrentHp();
    const SDL_Rect heartSource = cutRect(SpriteCutID::Heart_0);

    for (int i = 0; i < heartCount; ++i)
    {
        // Cada posição do Heart.png (0 a 4) equivale a meio coração, então 0 HP = posição 0 e HP cheio = posição 4.
        const int hpInHeart = std::clamp(currentHp - i * HP_PER_HEART, 0, HP_PER_HEART);
        const int frame = hpInHeart * 2;
        const auto cutId = static_cast<SpriteCutID>(std::to_underlying(SpriteCutID::Heart_0) + frame);

        const SDL_Rect source = cutRect(cutId);
        const SDL_Rect destination{
            HUD_MARGIN + i * heartSource.w * HUD_SCALE,
            HUD_MARGIN,
            heartSource.w * HUD_SCALE,
            heartSource.h * HUD_SCALE};
        TextureManager::drawInterface(heartTexture, source, destination, HUD_TINT);
    }
}

void GameWorld::drawCoins()
{
    if (scoreFont == nullptr)
    {
        scoreFont = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", 24);
        if (scoreFont == nullptr)
        {
            return;
        }
    }

    SDL_Texture *coinTexture = TextureManager::load(SpriteID::Coin);
    if (coinTexture == nullptr)
    {
        return;
    }

    const int coins = player->getCoins();
    if (coins != renderedCoins)
    {
        if (scoreTexture != nullptr)
        {
            SDL_DestroyTexture(scoreTexture);
        }

        scoreTexture = TextureManager::createTextTexture(scoreFont, std::to_string(coins), SDL_Color{255, 255, 255, 255});
        renderedCoins = scoreTexture != nullptr ? coins : -1;
    }

    const SDL_Rect coinSource = cutRect(SpriteCutID::Coin1);
    const SDL_Rect heartSource = cutRect(SpriteCutID::Heart_0);
    const SDL_Rect coinDestination{
        HUD_MARGIN,
        HUD_MARGIN + heartSource.h * HUD_SCALE + HUD_COIN_GAP,
        coinSource.w * HUD_SCALE,
        coinSource.h * HUD_SCALE};
    TextureManager::drawInterface(coinTexture, coinSource, coinDestination, HUD_TINT);

    if (scoreTexture != nullptr)
    {
        int textWidth = 0;
        int textHeight = 0;
        SDL_QueryTexture(scoreTexture, nullptr, nullptr, &textWidth, &textHeight);
        const int textX = coinDestination.x + coinDestination.w + HUD_COIN_GAP;
        const int textY = coinDestination.y + (coinDestination.h - textHeight) / 2;
        TextureManager::drawText(scoreTexture, textX, textY);
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
        if (!(*it) || !(*it)->isExist())
        {
            if (cameraTarget == it->get())
            {
                cameraTarget = nullptr;
            }
            if (player == it->get())
            {
                player = nullptr;
            }
            it = physicalObjects.erase(it);
            continue;
        }

        ++it;
    }
}
