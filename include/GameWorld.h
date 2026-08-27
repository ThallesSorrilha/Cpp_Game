#pragma once

#include <memory>
#include <list>

#include <SDL2/SDL_ttf.h>

#include "GameScene.h"
#include "PhysicalObject.h"
#include "TileMap.h"
#include "Camera.h"
#include "enums/Audio.h"

class Player;

class GameWorld : public GameScene
{
public:
    struct Config
    {
        GameScene::Config gameScene;
    };
    explicit GameWorld(const Config &config);
    virtual ~GameWorld() override;

    void handleInput() override;
    void update(float deltaTime) override;
    void draw() override;
    void drawScore(SDL_Renderer *renderer);

private:
    void processPendingAttackRequests();
    void processEnemyDrops();
    void killObjects();

    std::unique_ptr<TileMap> tileMap;
    std::list<std::unique_ptr<PhysicalObject>> physicalObjects;
    Player *player = nullptr;
    GameObject *cameraTarget = nullptr;
    Audio audio;
    TTF_Font *scoreFont = nullptr;
    SDL_Texture *scoreTexture = nullptr;
    int renderedCoins = -1;
};