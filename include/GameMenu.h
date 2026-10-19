#pragma once

#include <string>
#include <vector>
#include <SDL2/SDL.h>

#include "GameScene.h"

class GameMenu : public GameScene
{
public:
    struct Config
    {
        GameScene::Config gameScene;
        std::vector<std::string> options;
        int selectedOption = 0;
    };
    explicit GameMenu(const Config &config);

    void handleInput() override;
    void update(float deltaTime) override;
    void draw() override;
    Transition requestedTransition() const override;

private:
    std::vector<std::string> options;
    int selectedOption = 0;
    bool startPressed = false;
    bool transitionRequested = false;
    SDL_Texture *panelBackground = nullptr;
};