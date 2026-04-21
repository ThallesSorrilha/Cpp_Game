#pragma once

#include <memory>
#include <list>

#include "../GameScene/GameScene.h"
#include "../PhysicalObject/PhysicalObject.h"
#include "../TileMap/TileMap.h"
#include "../Camera/Camera.h"

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

private:
    void processPendingAttackRequests();

    std::unique_ptr<TileMap> tileMap;
    std::list<std::unique_ptr<PhysicalObject>> physicalObjects;
    GameObject *cameraTarget = nullptr;
};