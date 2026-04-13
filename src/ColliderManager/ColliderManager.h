#pragma once

#include "../utils/Vector2D.h"

class ColliderBox;
class TileMap;

class ColliderManager
{
public:
    static void resolveMovementAgainstTileMap(
        Vector2D &position, const ColliderBox &colliderBox, const TileMap &tileMap);
};