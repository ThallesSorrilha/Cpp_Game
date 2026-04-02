#pragma once

#include <string>

#include <SDL2/SDL.h>

#include "../TmxLoader/TmxLoader.h"

class TileMap
{
public:
    struct Config
    {
        std::string tmxFilePath;
    };

    explicit TileMap(const Config &config);
    virtual ~TileMap() = default;

    bool init();
    void update(float deltaTime);
    void draw();
    void shutdown();
    float getWidthInBlocks() const;
    float getHeightInBlocks() const;

private:
    std::string tmxFilePath;
    TmxMapData mapData;
    SDL_Texture *tilesetTexture = nullptr;
    int tilesetColumns = 0;
};
