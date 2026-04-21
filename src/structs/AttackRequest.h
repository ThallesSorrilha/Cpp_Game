#pragma once

#include <cstdint>

#include "../utils/Vector2D.h"

struct AttackRequest
{
    Vector2D position = {0.0f, 0.0f};
    Vector2D direction = {0.0f, 0.0f};
    int damage = 0;
    std::uint32_t collisionLayer = 0;
    std::uint32_t collisionMask = 0;
};