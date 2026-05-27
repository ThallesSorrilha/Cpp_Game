#include "../include/TileMap.h"

#include <cmath>
#include <iostream>
#include <stdexcept>

#include "../include/TextureManager.h"
#include "../include/definitions/Definitions.h"

namespace
{
    int findTilesetIndex(const std::vector<TmxTilesetData> &tilesets, int gid)
    {
        int index = -1;
        for (std::size_t i = 0; i < tilesets.size(); ++i)
        {
            if (gid >= tilesets[i].firstGid)
            {
                index = static_cast<int>(i);
            }
            else
            {
                break;
            }
        }
        return index;
    }
}

TileMap::TileMap(const Config &config)
    : mapID(config.mapID)
{
    if (!TmxLoader::load(mapID, mapData))
    {
        throw std::runtime_error("TileMap ctor error: failed to load TMX data");
    }

    tilesetTextures.clear();
    tilesetTextures.reserve(mapData.tilesets.size());

    for (auto &tileset : mapData.tilesets)
    {
        SDL_Texture *tilesetTexture = TextureManager::load(tileset.spriteID);
        if (tilesetTexture == nullptr)
        {
            throw std::runtime_error("TileMap ctor error: failed to load tileset texture");
        }

        if (tileset.columns <= 0)
        {
            int textureWidth = 0;
            int textureHeight = 0;

            if (SDL_QueryTexture(tilesetTexture, nullptr, nullptr, &textureWidth, &textureHeight) != 0)
            {
                throw std::runtime_error("TileMap ctor error: failed to query texture size");
            }

            if (tileset.tileWidth <= 0)
            {
                throw std::runtime_error("TileMap ctor error: invalid tile width");
            }

            tileset.columns = textureWidth / tileset.tileWidth;
        }

        if (tileset.columns <= 0)
        {
            throw std::runtime_error("TileMap ctor error: invalid tileset columns");
        }

        tilesetTextures.push_back(tilesetTexture);
    }
}

TileMap::~TileMap()
{
    mapData.layers.clear();
    mapData.tilesets.clear();
    tilesetTextures.clear();
}

void TileMap::update(float deltaTime) { (void)deltaTime; }

void TileMap::draw()
{
    if (tilesetTextures.empty())
    {
        return;
    }

    const float tileWidthInBlocks = static_cast<float>(mapData.tileWidth) / static_cast<float>(PIXELS_PER_TILE);
    const float tileHeightInBlocks = static_cast<float>(mapData.tileHeight) / static_cast<float>(PIXELS_PER_TILE);

    const std::size_t startLayerIndex = mapData.hasCollisionLayer ? 1u : 0u;
    for (std::size_t layerIndex = startLayerIndex; layerIndex < mapData.layers.size(); ++layerIndex)
    {
        const auto &layerData = mapData.layers[layerIndex];
        for (int y = 0; y < mapData.height; ++y)
        {
            for (int x = 0; x < mapData.width; ++x)
            {
                const std::size_t tileIndex = static_cast<std::size_t>(y) * static_cast<std::size_t>(mapData.width) + static_cast<std::size_t>(x);
                if (tileIndex >= layerData.size())
                {
                    continue;
                }

                const int gid = layerData[tileIndex];
                if (gid == 0)
                {
                    continue;
                }

                const int tilesetIndex = findTilesetIndex(mapData.tilesets, gid);
                if (tilesetIndex < 0)
                {
                    continue;
                }

                const auto &tileset = mapData.tilesets[static_cast<std::size_t>(tilesetIndex)];
                const int localId = gid - tileset.firstGid;
                if (localId < 0 || tileset.columns <= 0)
                {
                    continue;
                }

                const int row = localId / tileset.columns;
                const int col = localId % tileset.columns;

                TextureManager::drawTile(
                    tilesetTextures[static_cast<std::size_t>(tilesetIndex)],
                    static_cast<float>(x) * tileWidthInBlocks,
                    static_cast<float>(y) * tileHeightInBlocks,
                    tileWidthInBlocks,
                    tileHeightInBlocks,
                    row,
                    col);
            }
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

std::uint32_t TileMap::getCollisionLayer() const
{
    return collisionLayer;
}

bool TileMap::isCollisionTile(int tileX, int tileY) const
{
    if (!mapData.hasCollisionLayer)
    {
        return false;
    }

    if (tileX < 0 || tileY < 0 || tileX >= mapData.width || tileY >= mapData.height)
    {
        return true;
    }

    if (mapData.layers.empty())
    {
        return false;
    }

    const std::size_t index = static_cast<std::size_t>(tileY) * static_cast<std::size_t>(mapData.width) + static_cast<std::size_t>(tileX);
    const auto &collisionLayer = mapData.layers.front();
    if (index >= collisionLayer.size())
    {
        return true;
    }

    return collisionLayer[index] != 0;
}
