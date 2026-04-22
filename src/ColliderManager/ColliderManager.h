#pragma once

#include <list>
#include <memory>

#include "../utils/Vector2D.h"
#include "../structs/MapCollisionRect.h"
#include "../PhysicalObject/PhysicalObject.h"
#include "../ColliderBox/ColliderBox.h"
#include "../TileMap/TileMap.h"

class ColliderManager
{
public:
    static bool resolveMovementAgainstTileMap(
        Vector2D &position, ColliderBox &colliderBox, const TileMap &tileMap);
    static bool detectCollisionBetweenObjects(const ColliderBox &colliderBox, const ColliderBox &otherColliderBox);
    static Vector2D calculateDirectionBetweenObjects(const ColliderBox &colliderBox, const ColliderBox &otherColliderBox);
    static void detectObjectCollisions(const std::list<std::unique_ptr<PhysicalObject>> &objects);
    
private:
    static bool resolveAxisMapCollision(
        const MapCollisionRect &mapCollisionRect, Vector2D &position,
        bool &isXCorrect, bool &isYCorrect);
    static MapCollisionRect detectCollisionAgainstTileMap(
        const Vector2D &position, const ColliderBox &colliderBox, const TileMap &tileMap);
};