#include "Camera.h"

#include <algorithm>

Camera::Camera(const Config &config)
    : viewportSizeBlocks(config.viewportSizeBlocks),
      worldSizeBlocks(config.worldSizeBlocks),
      position(config.position)
{
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

const Vector2D &Camera::getPosition() const
{
    return position;
}

void Camera::clampToWorldBounds()
{
    const float maxX = worldSizeBlocks.x - viewportSizeBlocks.x;
    const float maxY = worldSizeBlocks.y - viewportSizeBlocks.y;

    if (maxX <= 0.0f)
    {
        position.x = 0.0f;
    }
    else
    {
        position.x = std::clamp(position.x, 0.0f, maxX);
    }

    if (maxY <= 0.0f)
    {
        position.y = 0.0f;
    }
    else
    {
        position.y = std::clamp(position.y, 0.0f, maxY);
    }
}
