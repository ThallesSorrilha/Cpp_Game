#pragma once

class GameScene
{
public:
    struct Config
    {
        bool initialized = false;
        bool paused = false;
    };
    explicit GameScene(const Config &config);
    virtual ~GameScene() = default;

    virtual void handleInput() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void draw() = 0;

protected:
    bool initialized;
    bool paused;
};