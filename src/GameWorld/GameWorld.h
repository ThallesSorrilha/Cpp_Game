#pragma once
#include <memory>
#include <list>
#include "../GameScene/GameScene.h"
#include "../GameObject/GameObject.h"
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

    bool init() override;
    void handleInput() override;
    void update(float deltaTime) override;
    void draw() override;
    void shutdown() override;

private:
    std::unique_ptr<TileMap> tileMap;
    std::list<std::unique_ptr<GameObject>> gameObjects; // list de unique ptr
    GameObject *cameraTarget = nullptr;
};