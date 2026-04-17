#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <SDL2/SDL.h>

#include "../TmxLoader/TmxLoader.h"
#include "../enums/MapID.h"
#include "../enums/LayerID.h"

class TileMap
{
public:
    struct Config
    {
        MapID mapID;
    };

    explicit TileMap(const Config &config);
    virtual ~TileMap();

    void update(float deltaTime);
    void draw();
    float getWidthInBlocks() const;
    float getHeightInBlocks() const;
    std::uint32_t getCollisionLayer() const;
    bool isCollisionTile(int tileX, int tileY) const;

private:
    TmxMapData mapData;
    SDL_Texture *tilesetTexture = nullptr;
    int tilesetColumns = 0;
    MapID mapID;
    std::uint32_t collisionLayer = toMask(LayerID::World);
};
