#pragma once

#include "../utils/Vector2D.h"

class ColliderBox;
class TileMap;

class ColliderManager
{
public:
    static Vector2D resolveMovementAgainstTileMap(
        const Vector2D &currentObjectPosition,
        const Vector2D &desiredDelta,
        const ColliderBox &colliderBox,
        const TileMap &tileMap);

private:
    static void resolveAxisX(
        Vector2D &resolvedObjectPosition,
        float deltaX,
        const ColliderBox &colliderBox,
        const TileMap &tileMap);

    static void resolveAxisY(
        Vector2D &resolvedObjectPosition,
        float deltaY,
        const ColliderBox &colliderBox,
        const TileMap &tileMap);
};