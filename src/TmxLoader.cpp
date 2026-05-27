#include "../include/TmxLoader.h"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <string_view>
#include <tinyxml2.h>
#include <utility>

#include "../include/definitions/MapArray.h"
#include "../include/definitions/SpriteArray.h"

namespace
{
    std::string trim(const std::string &value)
    {
        const auto first = std::find_if_not(value.begin(), value.end(), [](unsigned char ch)
                                            { return std::isspace(ch) != 0; });
        if (first == value.end())
        {
            return std::string();
        }

        const auto last = std::find_if_not(value.rbegin(), value.rend(), [](unsigned char ch)
                                           { return std::isspace(ch) != 0; })
                              .base();

        return std::string(first, last);
    }

    bool parseCsv(const std::string &csvText, std::vector<int> &outValues)
    {
        outValues.clear();

        std::stringstream stream(csvText);
        std::string token;
        while (std::getline(stream, token, ','))
        {
            token = trim(token);
            if (token.empty())
            {
                continue;
            }

            try
            {
                outValues.push_back(std::stoi(token));
            }
            catch (const std::exception &)
            {
                return false;
            }
        }

        return true;
    }

    const tinyxml2::XMLElement *findLayerByName(const tinyxml2::XMLElement *mapElement, const std::string &name)
    {
        for (const tinyxml2::XMLElement *layerElement = mapElement->FirstChildElement("layer");
             layerElement != nullptr;
             layerElement = layerElement->NextSiblingElement("layer"))
        {
            const char *layerNameCStr = layerElement->Attribute("name");
            if (layerNameCStr == nullptr)
            {
                continue;
            }

            if (name == layerNameCStr)
            {
                return layerElement;
            }
        }

        return nullptr;
    }

    bool parseLayerData(const tinyxml2::XMLElement *layerElement,
                        const std::string &layerName,
                        std::size_t expectedTileCount,
                        std::vector<int> &outLayerData)
    {
        const tinyxml2::XMLElement *dataElement = layerElement->FirstChildElement("data");
        if (dataElement == nullptr)
        {
            std::cerr << "TMX parse error: <data> missing in layer '" << layerName << "'" << std::endl;
            return false;
        }

        const char *encoding = dataElement->Attribute("encoding");
        if (encoding == nullptr || std::string(encoding) != "csv")
        {
            std::cerr << "TMX parse error: only CSV encoding is supported in layer '" << layerName << "'" << std::endl;
            return false;
        }

        const char *csvText = dataElement->GetText();
        if (csvText == nullptr)
        {
            std::cerr << "TMX parse error: CSV data missing in layer '" << layerName << "'" << std::endl;
            return false;
        }

        if (!parseCsv(csvText, outLayerData))
        {
            std::cerr << "TMX parse error: invalid CSV tile data in layer '" << layerName << "'" << std::endl;
            return false;
        }

        if (outLayerData.size() != expectedTileCount)
        {
            std::cerr << "TMX parse error: tile count mismatch in layer '" << layerName
                      << "'. expected " << expectedTileCount
                      << " got " << outLayerData.size() << std::endl;
            return false;
        }

        return true;
    }

    bool loadTilesetData(const std::filesystem::path &tmxPath,
                         const tinyxml2::XMLElement *tilesetElement,
                         int defaultTileWidth,
                         int defaultTileHeight,
                         TmxTilesetData &outTileset)
    {
        outTileset = TmxTilesetData{};
        outTileset.firstGid = tilesetElement->IntAttribute("firstgid", 1);

        const char *tmxTilesetSource = tilesetElement->Attribute("source");
        if (tmxTilesetSource == nullptr)
        {
            std::cerr << "TMX parse error: tileset source missing" << std::endl;
            return false;
        }

        const std::filesystem::path tmxTilesetPath = (tmxPath.parent_path() / tmxTilesetSource).lexically_normal();

        tinyxml2::XMLDocument tmxTilesetDocument;
        if (tmxTilesetDocument.LoadFile(tmxTilesetPath.string().c_str()) != tinyxml2::XML_SUCCESS)
        {
            std::cerr << "tmxTileset load error: " << tmxTilesetPath << std::endl;
            return false;
        }

        const tinyxml2::XMLElement *tmxTilesetTilesetElement = tmxTilesetDocument.FirstChildElement("tileset");
        if (tmxTilesetTilesetElement == nullptr)
        {
            std::cerr << "tmxTileset parse error: <tileset> missing" << std::endl;
            return false;
        }

        outTileset.tileWidth = tmxTilesetTilesetElement->IntAttribute("tilewidth", defaultTileWidth);
        outTileset.tileHeight = tmxTilesetTilesetElement->IntAttribute("tileheight", defaultTileHeight);
        outTileset.columns = tmxTilesetTilesetElement->IntAttribute("columns", 0);

        const tinyxml2::XMLElement *imageElement = tmxTilesetTilesetElement->FirstChildElement("image");
        if (imageElement == nullptr)
        {
            std::cerr << "tmxTileset parse error: <image> missing" << std::endl;
            return false;
        }

        const char *imageSource = imageElement->Attribute("source");
        if (imageSource == nullptr)
        {
            std::cerr << "tmxTileset parse error: image source missing" << std::endl;
            return false;
        }

        const std::filesystem::path imagePath = (tmxTilesetPath.parent_path() / imageSource).lexically_normal();
        outTileset.imagePath = imagePath.generic_string();

        bool found = false;
        for (size_t i = 0; i < SpriteArray.size(); ++i)
        {
            if (SpriteArray[i] == outTileset.imagePath)
            {
                outTileset.spriteID = static_cast<SpriteID>(i);
                found = true;
                break;
            }
        }
        if (!found)
        {
            std::cerr << "Image not found in SpriteArray: " << outTileset.imagePath << std::endl;
            return false;
        }

        return true;
    }
}

bool TmxLoader::load(const MapID &mapID, TmxMapData &outMapData)
{
    const std::string_view &tmxFilePath = MapArray[std::to_underlying(mapID)];
    outMapData = TmxMapData{};

    tinyxml2::XMLDocument tmxDocument;
    if (tmxDocument.LoadFile(std::string(tmxFilePath).c_str()) != tinyxml2::XML_SUCCESS)
    {
        std::cerr << "TMX load error: " << tmxFilePath << std::endl;
        return false;
    }

    const tinyxml2::XMLElement *mapElement = tmxDocument.FirstChildElement("map");
    if (mapElement == nullptr)
    {
        std::cerr << "TMX parse error: <map> missing" << std::endl;
        return false;
    }

    outMapData.width = mapElement->IntAttribute("width", 0);
    outMapData.height = mapElement->IntAttribute("height", 0);
    outMapData.tileWidth = mapElement->IntAttribute("tilewidth", 16);
    outMapData.tileHeight = mapElement->IntAttribute("tileheight", 16);

    if (outMapData.width <= 0 || outMapData.height <= 0)
    {
        std::cerr << "TMX parse error: invalid map size" << std::endl;
        return false;
    }

    const std::filesystem::path tmxPath(tmxFilePath);
    outMapData.tilesets.clear();
    for (const tinyxml2::XMLElement *tilesetElement = mapElement->FirstChildElement("tileset");
         tilesetElement != nullptr;
         tilesetElement = tilesetElement->NextSiblingElement("tileset"))
    {
        TmxTilesetData tilesetData;
        if (!loadTilesetData(tmxPath, tilesetElement, outMapData.tileWidth, outMapData.tileHeight, tilesetData))
        {
            return false;
        }
        outMapData.tilesets.push_back(std::move(tilesetData));
    }

    if (outMapData.tilesets.empty())
    {
        std::cerr << "TMX parse error: no tilesets found" << std::endl;
        return false;
    }

    std::sort(outMapData.tilesets.begin(), outMapData.tilesets.end(),
              [](const TmxTilesetData &a, const TmxTilesetData &b)
              { return a.firstGid < b.firstGid; });

    const std::size_t expectedTileCount = static_cast<std::size_t>(outMapData.width) * static_cast<std::size_t>(outMapData.height);

    outMapData.layers.clear();
    outMapData.hasCollisionLayer = false;

    const std::vector<std::string> layerOrder = {
        "collision",
        "floor",
        "decoration",
        "decoration_2",
        "ceil"};

    for (const auto &layerName : layerOrder)
    {
        const tinyxml2::XMLElement *layerElement = findLayerByName(mapElement, layerName);
        if (layerElement == nullptr)
        {
            continue;
        }

        std::vector<int> layerData;
        if (!parseLayerData(layerElement, layerName, expectedTileCount, layerData))
        {
            return false;
        }

        if (layerName == "collision")
        {
            outMapData.hasCollisionLayer = true;
        }

        outMapData.layers.push_back(std::move(layerData));
    }

    if (outMapData.layers.empty())
    {
        std::cerr << "TMX parse error: no supported layers found" << std::endl;
        return false;
    }

    return true;
}
