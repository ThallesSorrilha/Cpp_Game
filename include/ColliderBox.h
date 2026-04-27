#pragma once

#include "utils/Vector2D.h"

class ColliderBox
{
public:
    struct Config
    {
        std::uint32_t collisionLayer = 0;
        std::uint32_t collisionMask = 0;
        bool enabled = true;
        Vector2D offset = {0.04f, 0.04f};
        Vector2D size = {0.92f, 0.92f};
    };

    explicit ColliderBox(const Config &config);
    ~ColliderBox() = default;

    void setEnabled(bool newEnabled);
    bool isEnabled() const;

    void setCollisionLayer(std::uint32_t newCollisionLayer);
    std::uint32_t getCollisionLayer() const;

    void setCollisionMask(std::uint32_t newCollisionMask);
    std::uint32_t getCollisionMask() const;

    void setOffset(const Vector2D &newOffset);
    const Vector2D &getOffset() const;

    void setSize(const Vector2D &newSize);
    const Vector2D &getSize() const;

    void syncToObjectPosition(const Vector2D &objectPosition);
    const Vector2D &getPosition() const;
    
private:
    std::uint32_t collisionLayer;
    std::uint32_t collisionMask;
    bool enabled;
    Vector2D offset;
    Vector2D position;
    Vector2D size;
};
