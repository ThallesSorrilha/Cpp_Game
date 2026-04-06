#include "ColliderManager.h"

#include "../ColliderBox/ColliderBox.h"
#include "../TileMap/TileMap.h"

namespace
{
    constexpr float kSeparationEpsilon = 0.00f;
}

Vector2D ColliderManager::resolveMovementAgainstTileMap(
    const Vector2D &currentObjectPosition,
    const Vector2D &desiredDelta,
    const ColliderBox &colliderBox,
    const TileMap &tileMap)
{
    Vector2D resolvedObjectPosition = currentObjectPosition;

    if (!colliderBox.isEnabled())
    {
        return resolvedObjectPosition + desiredDelta;
    }

    resolvedObjectPosition.x += desiredDelta.x;
    resolveAxisX(resolvedObjectPosition, desiredDelta.x, colliderBox, tileMap);

    resolvedObjectPosition.y += desiredDelta.y;
    resolveAxisY(resolvedObjectPosition, desiredDelta.y, colliderBox, tileMap);

    return resolvedObjectPosition;
}

void ColliderManager::resolveAxisX(
    Vector2D &resolvedObjectPosition,
    float deltaX,
    const ColliderBox &colliderBox,
    const TileMap &tileMap)
{
    if (deltaX == 0.0f)
    {
        return;
    }

    const Vector2D &offset = colliderBox.getOffset();
    const Vector2D &size = colliderBox.getSize();

    const float colliderX = resolvedObjectPosition.x + offset.x;
    const float colliderY = resolvedObjectPosition.y + offset.y;

    const std::vector<TileMap::CollisionTileInfo> collisions =
        tileMap.getCollidingTilesAtWorld(colliderX, colliderY, size.x, size.y);

    if (collisions.empty())
    {
        return;
    }

    if (deltaX > 0.0f)
    {
        float correctedObjectX = resolvedObjectPosition.x;
        for (const TileMap::CollisionTileInfo &tile : collisions)
        {
            const float candidateObjectX = tile.minX - offset.x - size.x - kSeparationEpsilon;
            if (candidateObjectX < correctedObjectX)
            {
                correctedObjectX = candidateObjectX;
            }
        }
        resolvedObjectPosition.x = correctedObjectX;
    }
    else
    {
        float correctedObjectX = resolvedObjectPosition.x;
        for (const TileMap::CollisionTileInfo &tile : collisions)
        {
            const float candidateObjectX = tile.maxX - offset.x + kSeparationEpsilon;
            if (candidateObjectX > correctedObjectX)
            {
                correctedObjectX = candidateObjectX;
            }
        }
        resolvedObjectPosition.x = correctedObjectX;
    }
}

void ColliderManager::resolveAxisY(
    Vector2D &resolvedObjectPosition,
    float deltaY,
    const ColliderBox &colliderBox,
    const TileMap &tileMap)
{
    if (deltaY == 0.0f)
    {
        return;
    }

    const Vector2D &offset = colliderBox.getOffset();
    const Vector2D &size = colliderBox.getSize();

    const float colliderX = resolvedObjectPosition.x + offset.x;
    const float colliderY = resolvedObjectPosition.y + offset.y;

    const std::vector<TileMap::CollisionTileInfo> collisions =
        tileMap.getCollidingTilesAtWorld(colliderX, colliderY, size.x, size.y);

    if (collisions.empty())
    {
        return;
    }

    if (deltaY > 0.0f)
    {
        float correctedObjectY = resolvedObjectPosition.y;
        for (const TileMap::CollisionTileInfo &tile : collisions)
        {
            const float candidateObjectY = tile.minY - offset.y - size.y - kSeparationEpsilon;
            if (candidateObjectY < correctedObjectY)
            {
                correctedObjectY = candidateObjectY;
            }
        }
        resolvedObjectPosition.y = correctedObjectY;
    }
    else
    {
        float correctedObjectY = resolvedObjectPosition.y;
        for (const TileMap::CollisionTileInfo &tile : collisions)
        {
            const float candidateObjectY = tile.maxY - offset.y + kSeparationEpsilon;
            if (candidateObjectY > correctedObjectY)
            {
                correctedObjectY = candidateObjectY;
            }
        }
        resolvedObjectPosition.y = correctedObjectY;
    }
}