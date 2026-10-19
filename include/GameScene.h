#pragma once

class GameScene
{
public:
    enum class Transition
    {
        None,
        Menu,
        World
    };

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
    virtual Transition requestedTransition() const { return Transition::None; }
    void synchronizeInputState();

protected:
    bool initialized;
    bool paused;
    bool spaceWasDown = false;
};