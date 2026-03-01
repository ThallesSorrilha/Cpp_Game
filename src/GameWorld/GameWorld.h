#pragma once
#include "GameScene/GameScene.h"

class MenuScene : public GameScene
{
public:
    bool init() override;
    void handleInput() override;
    void update() override;
    void draw() override;
    void shutdown() override;
};