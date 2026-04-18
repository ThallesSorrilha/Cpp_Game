#pragma once

#include <string>
#include <vector>

#include "../enums/MapID.h"
#include "../enums/TextureID.h"

struct TmxTilesetData
{
    int firstGid = 1;
    int tileWidth = 16;
    int tileHeight = 16;
    int columns = 0;
    TextureID textureID;
    std::string imagePath;
};

struct TmxMapData
{
    int width = 0;
    int height = 0;
    int tileWidth = 16;
    int tileHeight = 16;
    std::vector<int> visualLayerData;
    std::vector<int> collisionLayerData;
    TmxTilesetData tileset;
};

class TmxLoader
{
public:
    static bool load(const MapID &mapID, TmxMapData &outMapData);
};
