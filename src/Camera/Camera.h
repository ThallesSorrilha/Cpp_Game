#pragma once

#include "../utils/Vector2D.h"

class Camera
{
public:
    struct Config
    {
        Vector2D viewportSizeBlocks = {0.0f, 0.0f};
        Vector2D worldSizeBlocks = {0.0f, 0.0f};
        Vector2D position = {0.0f, 0.0f};
    };

    explicit Camera(const Config &config);

    void setViewportSize(const Vector2D &newViewportSizeBlocks);
    void setWorldSize(const Vector2D &newWorldSizeBlocks);
    void setPosition(const Vector2D &newPosition);

    void follow(const Vector2D &targetPosition, const Vector2D &targetSize);
    const Vector2D &getPosition() const;

private:
    void clampToWorldBounds();

    Vector2D viewportSizeBlocks;
    Vector2D worldSizeBlocks;
    Vector2D position;
};