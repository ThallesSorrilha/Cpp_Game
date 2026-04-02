#include "TileMap.h"

#include <iostream>

#include "../TextureManager/TextureManager.h"
#include "../utils/Definitions.h"

TileMap::TileMap(const Config &config)
    : tmxFilePath(config.tmxFilePath)
{
}

bool TileMap::init()
{
    if (!TmxLoader::load(tmxFilePath, mapData))
    {
        return false;
    }

    tilesetTexture = TextureManager::load(mapData.tileset.imagePath);
    if (tilesetTexture == nullptr)
    {
        std::cerr << "TileMap init error: failed to load tileset texture: " << mapData.tileset.imagePath << std::endl;
        return false;
    }

    tilesetColumns = mapData.tileset.columns;
    if (tilesetColumns <= 0)
    {
        int textureWidth = 0;
        int textureHeight = 0;

        if (SDL_QueryTexture(tilesetTexture, nullptr, nullptr, &textureWidth, &textureHeight) != 0)
        {
            std::cerr << "TileMap init error: failed to query texture size" << std::endl;
            return false;
        }

        if (mapData.tileset.tileWidth <= 0)
        {
            std::cerr << "TileMap init error: invalid tile width" << std::endl;
            return false;
        }

        tilesetColumns = textureWidth / mapData.tileset.tileWidth;
    }

    if (tilesetColumns <= 0)
    {
        std::cerr << "TileMap init error: invalid tileset columns" << std::endl;
        return false;
    }

    return true;
}

void TileMap::update(float deltaTime) {}

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
            const int gid = mapData.layerData[static_cast<std::size_t>(y) * static_cast<std::size_t>(mapData.width) + static_cast<std::size_t>(x)];
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

void TileMap::shutdown()
{
    mapData.layerData.clear();
    tilesetTexture = nullptr;
    tilesetColumns = 0;
}

float TileMap::getWidthInBlocks() const
{
    return static_cast<float>(mapData.width * mapData.tileWidth) / static_cast<float>(PIXELS_PER_TILE);
}

float TileMap::getHeightInBlocks() const
{
    return static_cast<float>(mapData.height * mapData.tileHeight) / static_cast<float>(PIXELS_PER_TILE);
}
