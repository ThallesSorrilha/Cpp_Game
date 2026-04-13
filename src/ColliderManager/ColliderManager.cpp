#include "ColliderManager.h"

#include "../ColliderBox/ColliderBox.h"
#include "../TileMap/TileMap.h"

void ColliderManager::resolveMovementAgainstTileMap(
    Vector2D &position,
    const ColliderBox &colliderBox,
    const TileMap &tileMap)
{
    if (!colliderBox.isEnabled())
    {
        return;
    }

    bool isYCorrect = false;
    bool isXCorrect = false;

    const MapCollisionRect mapCollisionRect1 = detectCollisionAgainstTileMap(position, colliderBox, tileMap);
    resolveAxisMapCollision(mapCollisionRect1, position, isXCorrect, isYCorrect);

    const MapCollisionRect mapCollisionRect2 = detectCollisionAgainstTileMap(position, colliderBox, tileMap);
    resolveAxisMapCollision(mapCollisionRect2, position, isXCorrect, isYCorrect);
}

void ColliderManager::resolveAxisMapCollision(const MapCollisionRect &mapCollisionRect, Vector2D &position, bool &isXCorrect, bool &isYCorrect)
{
    if (isXCorrect && isYCorrect)
    {
        return;
    }

    if (mapCollisionRect.area == 0 || mapCollisionRect.overlapX <= 0 || mapCollisionRect.overlapY <= 0)
    {
        return;
    }

    if (mapCollisionRect.overlapY < mapCollisionRect.overlapX)
    {
        if (isYCorrect)
        {
            return;
        }

        if (mapCollisionRect.positiveDistY)
        {
            position.y += mapCollisionRect.overlapY;
        }
        else
        {
            position.y -= mapCollisionRect.overlapY;
        }
        isYCorrect = true;
    }
    else
    {
        if (isXCorrect)
        {
            return;
        }

        if (mapCollisionRect.positiveDistX)
        {
            position.x += mapCollisionRect.overlapX;
        }
        else
        {
            position.x -= mapCollisionRect.overlapX;
        }
        isXCorrect = true;
    }
}

MapCollisionRect ColliderManager::detectCollisionAgainstTileMap(
    const Vector2D &position,
    const ColliderBox &colliderBox,
    const TileMap &tileMap)
{
    if (!colliderBox.isEnabled() || colliderBox.getSize().x <= 0.0f || colliderBox.getSize().y <= 0.0f)
    {
        return MapCollisionRect();
    }

    const Vector2D &offset = colliderBox.getOffset();
    const Vector2D &size = colliderBox.getSize();
    const float colliderX = position.x + offset.x;
    const float colliderY = position.y + offset.y;

    const float xPosition = position.x + colliderBox.getOffset().x;
    const float yPosition = position.y + colliderBox.getOffset().y;
    constexpr float kSeparation = 0.0001f;

    const int minTileX = static_cast<int>(std::floor(xPosition));
    const int minTileY = static_cast<int>(std::floor(yPosition));
    const int maxTileX = static_cast<int>(std::floor(xPosition + colliderBox.getSize().x - kSeparation));
    const int maxTileY = static_cast<int>(std::floor(yPosition + colliderBox.getSize().y - kSeparation));

    MapCollisionRect mapCollisionRect;

    for (int tileY = minTileY; tileY <= maxTileY; ++tileY)
    {
        for (int tileX = minTileX; tileX <= maxTileX; ++tileX)
        {
            if (!tileMap.isCollisionTile(tileX, tileY))
            {
                continue;
            }

            const float distX = (colliderX + size.x / 2) - (tileX + 0.5);
            const bool positiveDistX = (distX > 0);
            const float distY = (colliderY + size.y / 2) - (tileY + 0.5);
            const bool positiveDistY = (distY > 0);

            const float halfW = size.x / 2 + 0.5;
            const float halfH = size.y / 2 + 0.5;

            const float overlapX = halfW - std::abs(distX);
            const float overlapY = halfH - std::abs(distY);

            if (overlapX <= 0 || overlapY <= 0)
            {
                continue;
            }

            const float area = overlapX * overlapY;
            if (area > mapCollisionRect.area)
            {
                mapCollisionRect = MapCollisionRect{
                    .x = tileX,
                    .y = tileY,
                    .area = area,
                    .overlapX = overlapX,
                    .overlapY = overlapY,
                    .positiveDistX = positiveDistX,
                    .positiveDistY = positiveDistY};
            }
        }
    }
    return mapCollisionRect;
}