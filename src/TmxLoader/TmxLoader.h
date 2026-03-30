#pragma once

#include <string>
#include <vector>

struct TmxTilesetData
{
    int firstGid = 1;
    int tileWidth = 16;
    int tileHeight = 16;
    int columns = 0;
    std::string imagePath;
};

struct TmxMapData
{
    int width = 0;
    int height = 0;
    int tileWidth = 16;
    int tileHeight = 16;
    std::vector<int> layerData;
    TmxTilesetData tileset;
};

class TmxLoader
{
public:
    static bool load(const std::string &tmxFilePath, TmxMapData &outMapData);
};
