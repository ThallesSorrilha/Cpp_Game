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
    virtual ~GameMenu() = default;

    bool init() override;
    void handleInput() override;
    void update(float deltaTime) override;
    void draw() override;
    void shutdown() override;

private:
    std::vector<std::string> options;
    int selectedOption = 0;
};