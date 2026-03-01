#pragma once

class GameScene
{
public:
    virtual ~GameScene() = default;

    virtual bool init() = 0;
    virtual void handleInput() = 0;
    virtual void update() = 0;
    virtual void draw() = 0;
    virtual void shutdown() = 0;
};