#pragma once

#include "../utils/Vector2D.h"

class ColliderBox
{
public:
    struct Config
    {
        int layerCollision = 1;
        int maskCollision = 1;
        bool enabled = true;
        Vector2D offset = {0.0f, 0.0f};
        Vector2D size = {1.0f, 1.0f};
    };

    explicit ColliderBox(const Config &config);
    ~ColliderBox() = default;

    void setEnabled(bool newEnabled);
    bool isEnabled() const;

    void setLayerCollision(int newLayerCollision);
    int getLayerCollision() const;

    void setMaskCollision(int newMaskCollision);
    int getMaskCollision() const;

    void setOffset(const Vector2D &newOffset);
    const Vector2D &getOffset() const;

    void setSize(const Vector2D &newSize);
    const Vector2D &getSize() const;

    void syncToObjectPosition(const Vector2D &objectPosition);
    const Vector2D &getPosition() const;

    bool intersectsAABB(const Vector2D &otherPosition, const Vector2D &otherSize) const;

private:
    int layerCollision;
    int maskCollision;
    bool enabled;
    Vector2D offset;
    Vector2D position;
    Vector2D size;
};
