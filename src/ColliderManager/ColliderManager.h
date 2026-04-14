#pragma once

#include <vector>

#include "../utils/Vector2D.h"
#include "../structs/MapCollisionRect.h"
class ColliderBox;
class TileMap;

class ColliderManager
{
public:
    static void resolveMovementAgainstTileMap(
        Vector2D &position, const ColliderBox &colliderBox, const TileMap &tileMap);

private:
    static std::vector<MapCollisionRect> detectCollisionAgainstTileMap(
        const Vector2D &position, const ColliderBox &colliderBox, const TileMap &tileMap);
};