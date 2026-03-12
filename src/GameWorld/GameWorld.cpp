#include "GameWorld.h"
#include <iostream>
#include "../GameManager/GameManager.h"

GameWorld::GameWorld(const Config &config)
    : GameScene(config.gameScene),
      worldId(config.worldId)
{
}

bool GameWorld::init()
{
    player = std::make_unique<Player>(Player::Config{.character = {.dynamicObject = {.gameObject = {.position = {0.0f, 0.0f}}}}});
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
    /*SDL_SetRenderDrawColor(renderer, 0, 100, 255, 255);
    SDL_RenderClear(renderer);*/
    if (player)
        player->draw();
}

void GameWorld::shutdown()
{
    if (player)
    {
        player->shutdown();
        player.reset();
    }
}
