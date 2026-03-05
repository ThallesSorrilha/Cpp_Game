#pragma once

#include "utils/Vector2D.h"

class GameObject
{
public:
    virtual ~GameObject() = default;

    virtual bool init() = 0;
    virtual void handleInput() = 0;
    virtual void update() = 0;
    virtual void draw() = 0;
    virtual void shutdown() = 0;

protected:
    Vector2D position;
    Vector2D size;
};