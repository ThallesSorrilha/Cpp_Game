#include "TmxLoader.h"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <iostream>
#include <sstream>

#include <tinyxml2.h>
#include "../maps/SpriteMap.h"

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
}

bool TmxLoader::load(const MapID &mapID, TmxMapData &outMapData)
{
    const std::string &tmxFilePath = MapMap.at(mapID);
    outMapData = TmxMapData{};

    tinyxml2::XMLDocument tmxDocument;
    if (tmxDocument.LoadFile(tmxFilePath.c_str()) != tinyxml2::XML_SUCCESS)
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

    const tinyxml2::XMLElement *tilesetElement = mapElement->FirstChildElement("tileset");
    if (tilesetElement == nullptr)
    {
        std::cerr << "TMX parse error: <tileset> missing" << std::endl;
        return false;
    }

    outMapData.tileset.firstGid = tilesetElement->IntAttribute("firstgid", 1);
    const char *tmxTilesetSource = tilesetElement->Attribute("source");
    if (tmxTilesetSource == nullptr)
    {
        std::cerr << "TMX parse error: tileset source missing" << std::endl;
        return false;
    }

    const std::filesystem::path tmxPath(tmxFilePath);
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

    outMapData.tileset.tileWidth = tmxTilesetTilesetElement->IntAttribute("tilewidth", outMapData.tileWidth);
    outMapData.tileset.tileHeight = tmxTilesetTilesetElement->IntAttribute("tileheight", outMapData.tileHeight);
    outMapData.tileset.columns = tmxTilesetTilesetElement->IntAttribute("columns", 0);

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
    outMapData.tileset.imagePath = imagePath.generic_string();

    bool found = false;
    for (const auto &[textureID, imagePath] : SpriteMap)
    {
        if (imagePath == outMapData.tileset.imagePath)
        {
            outMapData.tileset.textureID = textureID;
            found = true;
            break;
        }
    }
    if (!found)
    {
        std::cerr << "Image not found in SpriteMap: " << outMapData.tileset.imagePath << std::endl;
        return false;
    }

    const std::size_t expectedTileCount = static_cast<std::size_t>(outMapData.width) * static_cast<std::size_t>(outMapData.height);

    bool hasVisualLayer = false;
    bool hasCollisionLayer = false;

    for (const tinyxml2::XMLElement *layerElement = mapElement->FirstChildElement("layer");
         layerElement != nullptr;
         layerElement = layerElement->NextSiblingElement("layer"))
    {
        const char *layerNameCStr = layerElement->Attribute("name");
        if (layerNameCStr == nullptr)
        {
            continue;
        }

        const std::string layerName(layerNameCStr);
        std::vector<int> *targetLayerData = nullptr;

        if (layerName == "visual")
        {
            targetLayerData = &outMapData.visualLayerData;
            hasVisualLayer = true;
        }
        else if (layerName == "collision")
        {
            targetLayerData = &outMapData.collisionLayerData;
            hasCollisionLayer = true;
        }
        else
        {
            continue;
        }

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

        if (!parseCsv(csvText, *targetLayerData))
        {
            std::cerr << "TMX parse error: invalid CSV tile data in layer '" << layerName << "'" << std::endl;
            return false;
        }

        if (targetLayerData->size() != expectedTileCount)
        {
            std::cerr << "TMX parse error: tile count mismatch in layer '" << layerName
                      << "'. expected " << expectedTileCount
                      << " got " << targetLayerData->size() << std::endl;
            return false;
        }
    }

    if (!hasVisualLayer)
    {
        std::cerr << "TMX parse error: required layer 'visual' missing" << std::endl;
        return false;
    }

    if (!hasCollisionLayer)
    {
        std::cerr << "TMX parse error: required layer 'collision' missing" << std::endl;
        return false;
    }

    return true;
}
