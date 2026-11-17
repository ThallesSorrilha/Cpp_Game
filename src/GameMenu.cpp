#include "../include/GameMenu.h"

#include <array>
#include <iostream>

#include "../include/TextureManager.h"
#include "../include/definitions/Definitions.h"
#include "../include/enums/SpriteID.h"

namespace
{
    constexpr const char *FONT_PATH = "assets/Ui/Font/NormalFont.ttf";
    constexpr int FONT_SIZE = 22;

    constexpr float PANEL_WIDTH_RATIO = 0.80f;
    constexpr float PANEL_HEIGHT_RATIO = 0.80f;
    constexpr float PANEL_PADDING_RATIO = 0.05f;
    constexpr float TAB_HEIGHT_RATIO = 0.12f;
    constexpr float TAB_GAP_RATIO = 0.02f;
    constexpr float LINE_GAP_RATIO = 0.03f;

    constexpr SDL_Color TEXT_COLOR{40, 25, 10, 255};
    constexpr SDL_Color TAB_NORMAL_TINT{255, 255, 255, 255};
    constexpr SDL_Color TAB_SELECTED_TINT{255, 210, 140, 255};

    struct TabData
    {
        const char *title;
        std::array<const char *, 3> lines;
    };

    constexpr std::array<TabData, GameMenu::TAB_COUNT> TABS{{
        {"Inventario", {"Espada de ferro", "Pocao de vida", "Moedas: 0"}},
        {"Mapa", {"Floresta Norte", "Vila Insolitus", "Caverna Sombria"}},
        {"Controles", {"A / D - Trocar aba", "W A S D - Mover", "J - Atacar"}},
        {"Configuracoes", {"Volume musica: 100%", "Volume efeitos: 100%", "Tela cheia: nao"}},
    }};

    int ratioOf(int base, float ratio)
    {
        return static_cast<int>(static_cast<float>(base) * ratio);
    }
} // namespace

GameMenu::GameMenu(const Config &config)
    : GameScene(config.gameScene),
      panelBackground(TextureManager::load(SpriteID::NinePathPanelBackground)),
      buttonTexture(TextureManager::load(SpriteID::UiButtonNormal))
{
    const Uint8 *keyStates = SDL_GetKeyboardState(nullptr);
    for (int i = 0; i < SDL_NUM_SCANCODES; ++i)
    {
        previousKeys[i] = keyStates[i];
    }

    font = TTF_OpenFont(FONT_PATH, FONT_SIZE);
    if (font == nullptr)
    {
        std::cerr << "GameMenu could not open font " << FONT_PATH << ": " << TTF_GetError() << std::endl;
    }

    for (std::size_t i = 0; i < TAB_COUNT; ++i)
    {
        tabs[i].label = TextureManager::createTextTexture(font, TABS[i].title, TEXT_COLOR);
        for (const char *line : TABS[i].lines)
        {
            tabs[i].lines.push_back(TextureManager::createTextTexture(font, line, TEXT_COLOR));
        }
    }
}

GameMenu::~GameMenu()
{
    for (auto &tab : tabs)
    {
        if (tab.label != nullptr)
        {
            SDL_DestroyTexture(tab.label);
        }
        for (SDL_Texture *line : tab.lines)
        {
            if (line != nullptr)
            {
                SDL_DestroyTexture(line);
            }
        }
    }

    if (font != nullptr)
    {
        TTF_CloseFont(font);
    }
}

void GameMenu::handleInput()
{
    const Uint8 *keyStates = SDL_GetKeyboardState(nullptr);
    auto pressed = [&](SDL_Scancode key)
    {
        return keyStates[key] != 0 && previousKeys[key] == 0;
    };

    if (pressed(SDL_SCANCODE_SPACE))
    {
        transitionRequested = true;
    }

    if (pressed(SDL_SCANCODE_A) || pressed(SDL_SCANCODE_LEFT))
    {
        selectedTab = (selectedTab + TAB_COUNT - 1) % TAB_COUNT;
    }
    if (pressed(SDL_SCANCODE_D) || pressed(SDL_SCANCODE_RIGHT))
    {
        selectedTab = (selectedTab + 1) % TAB_COUNT;
    }

    for (int i = 0; i < SDL_NUM_SCANCODES; ++i)
    {
        previousKeys[i] = keyStates[i];
    }
}

GameScene::Transition GameMenu::requestedTransition() const
{
    return transitionRequested ? Transition::World : Transition::None;
}

void GameMenu::clearTransitionRequest()
{
    transitionRequested = false;
}

void GameMenu::update(float deltaTime) { (void)deltaTime; }

void GameMenu::draw()
{
    TextureManager::drawScreenOverlay(SDL_Color{0, 0, 0, 128});

    const int panelWidth = ratioOf(SCREEN_WIDTH, PANEL_WIDTH_RATIO);
    const int panelHeight = ratioOf(SCREEN_HEIGHT, PANEL_HEIGHT_RATIO);
    const SDL_Rect panel{
        (SCREEN_WIDTH - panelWidth) / 2,
        (SCREEN_HEIGHT - panelHeight) / 2,
        panelWidth,
        panelHeight};

    TextureManager::drawInterface(panelBackground, panel, SDL_Color{255, 255, 255, 255});

    const int padding = ratioOf(panelWidth, PANEL_PADDING_RATIO);
    const int tabHeight = ratioOf(panelHeight, TAB_HEIGHT_RATIO);
    const int lineGap = ratioOf(panelHeight, LINE_GAP_RATIO);

    const SDL_Rect inner{
        panel.x + padding,
        panel.y + padding,
        panel.w - 2 * padding,
        panel.h - 2 * padding};

    const SDL_Rect tabArea{inner.x, inner.y, inner.w, tabHeight};
    const SDL_Rect contentArea{
        inner.x,
        tabArea.y + tabArea.h + lineGap * 2,
        inner.w,
        inner.h - tabArea.h - lineGap * 2};

    drawTabs(tabArea);
    drawContent(contentArea);
}

void GameMenu::drawTabs(const SDL_Rect &area) const
{
    const int gap = ratioOf(area.w, TAB_GAP_RATIO);
    const int tabCount = static_cast<int>(TAB_COUNT);
    const int tabWidth = (area.w - gap * (tabCount - 1)) / tabCount;

    for (std::size_t i = 0; i < TAB_COUNT; ++i)
    {
        const SDL_Rect tabRect{
            area.x + static_cast<int>(i) * (tabWidth + gap),
            area.y,
            tabWidth,
            area.h};

        const bool selected = i == selectedTab;
        TextureManager::drawInterface(buttonTexture, tabRect, selected ? TAB_SELECTED_TINT : TAB_NORMAL_TINT);

        SDL_Texture *label = tabs[i].label;
        if (label != nullptr)
        {
            int labelWidth = 0;
            int labelHeight = 0;
            SDL_QueryTexture(label, nullptr, nullptr, &labelWidth, &labelHeight);
            const int x = tabRect.x + (tabRect.w - labelWidth) / 2;
            const int y = tabRect.y + (tabRect.h - labelHeight) / 2;
            TextureManager::drawText(label, x, y);
        }
    }
}

void GameMenu::drawContent(const SDL_Rect &area) const
{
    const int lineGap = ratioOf(area.h, LINE_GAP_RATIO);
    int y = area.y;

    for (SDL_Texture *line : tabs[selectedTab].lines)
    {
        if (line == nullptr)
        {
            continue;
        }

        TextureManager::drawText(line, area.x, y);

        int lineWidth = 0;
        int lineHeight = 0;
        SDL_QueryTexture(line, nullptr, nullptr, &lineWidth, &lineHeight);
        y += lineHeight + lineGap;
    }
}
