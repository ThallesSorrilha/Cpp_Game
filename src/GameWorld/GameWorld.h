#pragma once
#include <memory>
#include <vector>
#include "../GameScene/GameScene.h"
#include "../GameObject/GameObject.h"
#include "../TileMap/TileMap.h"

class GameWorld : public GameScene
{
public:
    struct Config
    {
        GameScene::Config gameScene;
        int worldId = 0;
    };
    explicit GameWorld(const Config &config);
    virtual ~GameWorld() = default;

    bool init() override;
    void handleInput() override;
    void update(float deltaTime) override;
    void draw() override;
    void shutdown() override;

private:
    int worldId;
    std::unique_ptr<TileMap> tileMap;
    std::vector<std::unique_ptr<GameObject>> gameObjects;
};