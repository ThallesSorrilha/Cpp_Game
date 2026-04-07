#include "TileMap.h"

#include <cmath>
#include <iostream>
#include <stdexcept>

#include "../TextureManager/TextureManager.h"
#include "../utils/Definitions.h"

TileMap::TileMap(const Config &config)
    : mapID(config.mapID)
{
    if (!TmxLoader::load(mapID, mapData))
    {
        throw std::runtime_error("TileMap ctor error: failed to load TMX data");
    }

    tilesetTexture = TextureManager::load(mapData.tileset.textureID);
    if (tilesetTexture == nullptr)
    {
        throw std::runtime_error("TileMap ctor error: failed to load tileset texture");
    }

    tilesetColumns = mapData.tileset.columns;
    if (tilesetColumns <= 0)
    {
        int textureWidth = 0;
        int textureHeight = 0;

        if (SDL_QueryTexture(tilesetTexture, nullptr, nullptr, &textureWidth, &textureHeight) != 0)
        {
            throw std::runtime_error("TileMap ctor error: failed to query texture size");
        }

        if (mapData.tileset.tileWidth <= 0)
        {
            throw std::runtime_error("TileMap ctor error: invalid tile width");
        }

        tilesetColumns = textureWidth / mapData.tileset.tileWidth;
    }

    if (tilesetColumns <= 0)
    {
        throw std::runtime_error("TileMap ctor error: invalid tileset columns");
    }
}

TileMap::~TileMap()
{
    mapData.visualLayerData.clear();
    mapData.collisionLayerData.clear();
    tilesetTexture = nullptr;
    tilesetColumns = 0;
}

void TileMap::update(float deltaTime)
{
    (void)deltaTime;
}

void TileMap::draw()
{
    if (tilesetTexture == nullptr)
    {
        return;
    }

    const float tileWidthInBlocks = static_cast<float>(mapData.tileWidth) / static_cast<float>(PIXELS_PER_TILE);
    const float tileHeightInBlocks = static_cast<float>(mapData.tileHeight) / static_cast<float>(PIXELS_PER_TILE);

    for (int y = 0; y < mapData.height; ++y)
    {
        for (int x = 0; x < mapData.width; ++x)
        {
            const int gid = mapData.visualLayerData[static_cast<std::size_t>(y) * static_cast<std::size_t>(mapData.width) + static_cast<std::size_t>(x)];
            if (gid == 0)
            {
                continue;
            }

            const int localId = gid - mapData.tileset.firstGid;
            if (localId < 0)
            {
                continue;
            }

            const int row = localId / tilesetColumns;
            const int col = localId % tilesetColumns;

            TextureManager::drawTile(
                tilesetTexture,
                static_cast<float>(x) * tileWidthInBlocks,
                static_cast<float>(y) * tileHeightInBlocks,
                tileWidthInBlocks,
                tileHeightInBlocks,
                row,
                col);
        }
    }
}

float TileMap::getWidthInBlocks() const
{
    return static_cast<float>(mapData.width * mapData.tileWidth) / static_cast<float>(PIXELS_PER_TILE);
}

float TileMap::getHeightInBlocks() const
{
    return static_cast<float>(mapData.height * mapData.tileHeight) / static_cast<float>(PIXELS_PER_TILE);
}

bool TileMap::isCollisionTile(int tileX, int tileY) const
{
    if (tileX < 0 || tileY < 0 || tileX >= mapData.width || tileY >= mapData.height)
    {
        return true;
    }

    const std::size_t index = static_cast<std::size_t>(tileY) * static_cast<std::size_t>(mapData.width) + static_cast<std::size_t>(tileX);
    return mapData.collisionLayerData[index] != 0;
}

bool TileMap::isCollisionAtWorld(float xPosition, float yPosition) const
{
    const int tileX = static_cast<int>(std::floor(xPosition));
    const int tileY = static_cast<int>(std::floor(yPosition));
    return isCollisionTile(tileX, tileY);
}

bool TileMap::intersectsCollisionAtWorld(float xPosition, float yPosition, float width, float height) const
{
    return !getCollidingTilesAtWorld(xPosition, yPosition, width, height).empty();
}

std::vector<TileMap::CollisionTileInfo> TileMap::getCollidingTilesAtWorld(float xPosition, float yPosition, float width, float height) const
{
    std::vector<CollisionTileInfo> collisions;

    if (width <= 0.0f || height <= 0.0f)
    {
        return collisions;
    }

    constexpr float epsilon = 0.02f;
    const int minTileX = static_cast<int>(std::floor(xPosition));
    const int minTileY = static_cast<int>(std::floor(yPosition));
    const int maxTileX = static_cast<int>(std::floor(xPosition + width - epsilon));
    const int maxTileY = static_cast<int>(std::floor(yPosition + height - epsilon));

    for (int tileY = minTileY; tileY <= maxTileY; ++tileY)
    {
        for (int tileX = minTileX; tileX <= maxTileX; ++tileX)
        {
            if (!isCollisionTile(tileX, tileY))
            {
                continue;
            }

            collisions.push_back(CollisionTileInfo{
                .tileX = tileX,
                .tileY = tileY,
                .minX = static_cast<float>(tileX),
                .minY = static_cast<float>(tileY),
                .maxX = static_cast<float>(tileX + 1),
                .maxY = static_cast<float>(tileY + 1)});
        }
    }

    return collisions;
}
