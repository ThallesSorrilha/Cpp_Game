#pragma once

#include <array>
#include <cstddef>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "GameScene.h"

class GameMenu : public GameScene
{
public:
    static constexpr std::size_t TAB_COUNT = 4;

    struct Config
    {
        GameScene::Config gameScene;
    };
    explicit GameMenu(const Config &config);
    ~GameMenu() override;
    GameMenu(const GameMenu &) = delete;
    GameMenu &operator=(const GameMenu &) = delete;

    void handleInput() override;
    void update(float deltaTime) override;
    void draw() override;
    Transition requestedTransition() const override;
    void clearTransitionRequest() override;

private:
    struct TabView
    {
        SDL_Texture *label = nullptr;
        std::vector<SDL_Texture *> lines;
    };

    void drawTabs(const SDL_Rect &area) const;
    void drawContent(const SDL_Rect &area) const;

    std::array<TabView, TAB_COUNT> tabs{};
    std::array<Uint8, SDL_NUM_SCANCODES> previousKeys{};
    std::size_t selectedTab = 0;
    bool transitionRequested = false;
    SDL_Texture *panelBackground = nullptr;
    SDL_Texture *buttonTexture = nullptr;
    TTF_Font *font = nullptr;
};