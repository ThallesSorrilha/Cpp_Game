#pragma once

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
    static void resolveAxisMapCollision(
        const MapCollisionRect &mapCollisionRect, Vector2D &position,
        bool &isXCorrect, bool &isYCorrect);

    static MapCollisionRect detectCollisionAgainstTileMap(
        const Vector2D &position, const ColliderBox &colliderBox, const TileMap &tileMap);
};