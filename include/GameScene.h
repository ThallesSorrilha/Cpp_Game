#pragma once

struct SDL_Renderer;

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
    virtual void drawUi(SDL_Renderer *renderer) { (void)renderer; }
    virtual Transition requestedTransition() const { return Transition::None; }
    virtual void clearTransitionRequest() {}
    void synchronizeInputState();
    void setPaused(bool value);
    bool isPaused() const;

protected:
    bool initialized;
    bool paused;
    bool spaceWasDown = false;
};