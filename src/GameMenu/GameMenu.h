#pragma once

#include <string>
#include <vector>

#include "../GameScene/GameScene.h"

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

private:
    std::vector<std::string> options;
    int selectedOption = 0;
};