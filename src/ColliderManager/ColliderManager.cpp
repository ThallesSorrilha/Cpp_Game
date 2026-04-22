#include "ColliderManager.h"

#include "../ColliderBox/ColliderBox.h"
#include "../TileMap/TileMap.h"

bool ColliderManager::resolveMovementAgainstTileMap(
    Vector2D &position,
    ColliderBox &colliderBox,
    const TileMap &tileMap)
{
    if (!colliderBox.isEnabled())
    {
        return false;
    }
    if (!LayerUtils::hasLayer(colliderBox.getCollisionMask(), tileMap.getCollisionLayer()))
    {
        return false;
    }

    bool isYCorrect = false;
    bool isXCorrect = false;

    const MapCollisionRect mapCollisionRect1 = detectCollisionAgainstTileMap(position, colliderBox, tileMap);
    if (mapCollisionRect1.area == 0)
        return false;
    resolveAxisMapCollision(mapCollisionRect1, position, isXCorrect, isYCorrect);

    const MapCollisionRect mapCollisionRect2 = detectCollisionAgainstTileMap(position, colliderBox, tileMap);
    if (mapCollisionRect2.area == 0)
        return true;
    resolveAxisMapCollision(mapCollisionRect2, position, isXCorrect, isYCorrect);

    colliderBox.syncToObjectPosition(position);
    return true;
}

bool ColliderManager::resolveAxisMapCollision(const MapCollisionRect &mapCollisionRect, Vector2D &position, bool &isXCorrect, bool &isYCorrect)
{
    if (isXCorrect && isYCorrect)
    {
        return false;
    }
    if (mapCollisionRect.area == 0 || mapCollisionRect.overlapX <= 0 || mapCollisionRect.overlapY <= 0)
    {
        return false;
    }

    if (mapCollisionRect.overlapY < mapCollisionRect.overlapX)
    {
        if (isYCorrect)
        {
            return false;
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
            return false;
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
    return true;
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

bool ColliderManager::detectCollisionBetweenObjects(const ColliderBox &colliderBox, const ColliderBox &otherColliderBox)
{
    //std::cout << "detectCollisionBetweenObjects" << std::endl;
    // debugar aqui
    if ((!colliderBox.isEnabled()) || (!otherColliderBox.isEnabled()))
    {
        return false;
    }

    //std::cout << "A(layer):" << static_cast<int>(LayerUtils::getLayer(colliderBox.getCollisionLayer())) << " - B(layer):" << static_cast<int>(LayerUtils::getLayer(otherColliderBox.getCollisionLayer())) << std::endl;

    if ((!LayerUtils::hasLayer(colliderBox.getCollisionMask(), otherColliderBox.getCollisionLayer())) && (!LayerUtils::hasLayer(otherColliderBox.getCollisionMask(), colliderBox.getCollisionLayer())))
    {
        //std::cout << "!red!" << std::endl;
        return false;
    }
    //std::cout << ".green." << std::endl;

    const Vector2D &position = colliderBox.getPosition();
    const Vector2D &size = colliderBox.getSize();
    const Vector2D &otherPosition = otherColliderBox.getPosition();
    const Vector2D &otherSize = otherColliderBox.getSize();

    const bool overlapX = (position.x < otherPosition.x + otherSize.x) && (position.x + size.x > otherPosition.x);
    const bool overlapY = (position.y < otherPosition.y + otherSize.y) && (position.y + size.y > otherPosition.y);
    return overlapX && overlapY;
}

Vector2D ColliderManager::calculateCollisionBetweenObjects(const ColliderBox &colliderBox, const ColliderBox &otherColliderBox)
{
    Vector2D overlap = {0.0f, 0.0f};

    if ((!colliderBox.isEnabled()) || (!otherColliderBox.isEnabled()))
    {
        return overlap;
    }
    if (!LayerUtils::hasLayer(colliderBox.getCollisionMask(), otherColliderBox.getCollisionLayer()))
    {
        return overlap;
    }

    const Vector2D &position = colliderBox.getPosition();
    const Vector2D &size = colliderBox.getSize();
    const Vector2D &otherPosition = otherColliderBox.getPosition();
    const Vector2D &otherSize = otherColliderBox.getSize();

    const float distX = (position.x + size.x / 2) - (otherPosition.x + otherSize.x / 2);
    const bool positiveDistX = (distX > 0);
    const float distY = (position.y + size.y / 2) - (otherPosition.y + otherSize.y / 2);
    const bool positiveDistY = (distY > 0);

    const float halfW = size.x / 2 + otherSize.x / 2;
    const float halfH = size.y / 2 + otherSize.y / 2;

    const float overlapX = halfW - std::abs(distX);
    const float overlapY = halfH - std::abs(distY);

    if (overlapX <= 0 || overlapY <= 0)
    {
        return overlap;
    }

    overlap = {overlapX, overlapY};
    if (!positiveDistX)
    {
        overlap.x = overlap.x * -1;
    }
    if (!positiveDistY)
    {
        overlap.y = overlap.y * -1;
    }

    return overlap;
}

// complexidade O^2/2
void ColliderManager::detectObjectCollisions(const std::list<std::unique_ptr<PhysicalObject>> &objects)
{
    for (auto itA = objects.begin(); itA != objects.end(); ++itA)
    {
        auto itB = std::next(itA);
        for (; itB != objects.end(); ++itB)
        {
            PhysicalObject *a = itA->get();
            PhysicalObject *b = itB->get();

            if (a == nullptr || b == nullptr)
            {
                continue;
            }

            ColliderBox *colliderA = a->getColliderBox();
            ColliderBox *colliderB = b->getColliderBox();
            if (colliderA == nullptr || colliderB == nullptr)
            {
                continue;
            }

            bool collision = detectCollisionBetweenObjects(*colliderA, *colliderB);
            if (!collision)
            {
                continue;
            }

            Vector2D overlap = calculateCollisionBetweenObjects(*colliderA, *colliderB);
            a->onCollision(*b, overlap);
            b->onCollision(*a, -overlap);
        }
    }
}
