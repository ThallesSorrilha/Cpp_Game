#pragma once

#include "../utils/Vector2D.h"
#include "../structs/MapCollisionRect.h"
#include "../DynamicObject/DynamicObject.h"
#include "../ColliderBox/ColliderBox.h"
#include "../TileMap/TileMap.h"

class ColliderManager
{
public:
    static bool resolveMovementAgainstTileMap(
        Vector2D &position, ColliderBox &colliderBox, const TileMap &tileMap);
    static bool detectCollisionBetweenObjects(
        const Vector2D &position, const ColliderBox &colliderBox,
        const Vector2D &otherPosition, const ColliderBox &otherColliderBox);

private:
    static bool resolveAxisMapCollision(
        const MapCollisionRect &mapCollisionRect, Vector2D &position,
        bool &isXCorrect, bool &isYCorrect);

    static MapCollisionRect detectCollisionAgainstTileMap(
        const Vector2D &position, const ColliderBox &colliderBox, const TileMap &tileMap);
};