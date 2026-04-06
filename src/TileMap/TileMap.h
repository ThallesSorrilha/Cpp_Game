#pragma once

#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include "../TmxLoader/TmxLoader.h"
#include "../enums/MapID.h"

class TileMap
{
public:
    struct CollisionTileInfo
    {
        int tileX = 0;
        int tileY = 0;
        float minX = 0.0f;
        float minY = 0.0f;
        float maxX = 0.0f;
        float maxY = 0.0f;
    };

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
    bool isCollisionTile(int tileX, int tileY) const;
    bool isCollisionAtWorld(float xPosition, float yPosition) const;
    bool intersectsCollisionAtWorld(float xPosition, float yPosition, float width, float height) const;
    std::vector<CollisionTileInfo> getCollidingTilesAtWorld(float xPosition, float yPosition, float width, float height) const;

private:
    TmxMapData mapData;
    SDL_Texture *tilesetTexture = nullptr;
    int tilesetColumns = 0;
    MapID mapID;
};
