#include "Camera.h"

#include <algorithm>

void Camera::init(const Vector2D &newViewportSizeBlocks, const Vector2D &newWorldSizeBlocks)
{
    viewportSizeBlocks = newViewportSizeBlocks;
    worldSizeBlocks = newWorldSizeBlocks;
    clampToWorldBounds();
}

void Camera::setViewportSize(const Vector2D &newViewportSizeBlocks)
{
    viewportSizeBlocks = newViewportSizeBlocks;
    clampToWorldBounds();
}

void Camera::setWorldSize(const Vector2D &newWorldSizeBlocks)
{
    worldSizeBlocks = newWorldSizeBlocks;
    clampToWorldBounds();
}

void Camera::setPosition(const Vector2D &newPosition)
{
    position = newPosition;
    clampToWorldBounds();
}

void Camera::follow(const Vector2D &targetPosition, const Vector2D &targetSize)
{
    const float targetCenterX = targetPosition.x + (targetSize.x * 0.5f);
    const float targetCenterY = targetPosition.y + (targetSize.y * 0.5f);

    position.x = targetCenterX - (viewportSizeBlocks.x * 0.5f);
    position.y = targetCenterY - (viewportSizeBlocks.y * 0.5f);

    clampToWorldBounds();
}

const Vector2D &Camera::getPosition()
{
    return position;
}

void Camera::clampToWorldBounds()
{
    const float maxX = worldSizeBlocks.x - viewportSizeBlocks.x;
    const float maxY = worldSizeBlocks.y - viewportSizeBlocks.y;

    if (maxX <= 0.0f)
    {
        position.x = -viewportSizeBlocks.x / 2 + worldSizeBlocks.x / 2;
    }
    else
    {
        position.x = std::clamp(position.x, 0.0f, maxX);
    }

    if (maxY <= 0.0f)
    {
        position.y = -viewportSizeBlocks.y / 2 + worldSizeBlocks.y / 2;
    }
    else
    {
        position.y = std::clamp(position.y, 0.0f, maxY);
    }
}
