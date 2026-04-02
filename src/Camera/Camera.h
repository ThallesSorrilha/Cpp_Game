#pragma once

#include "../utils/Vector2D.h"

class Camera
{
public:
    static void init(const Vector2D &viewportSizeBlocks, const Vector2D &worldSizeBlocks);

    static void setViewportSize(const Vector2D &newViewportSizeBlocks);
    static void setWorldSize(const Vector2D &newWorldSizeBlocks);
    static void setPosition(const Vector2D &newPosition);

    static void follow(const Vector2D &targetPosition, const Vector2D &targetSize);
    static const Vector2D &getPosition();

private:
    static inline void clampToWorldBounds();

    static inline Vector2D viewportSizeBlocks = {0.0f, 0.0f};
    static inline Vector2D worldSizeBlocks = {0.0f, 0.0f};
    static inline Vector2D position = {0.0f, 0.0f};
};