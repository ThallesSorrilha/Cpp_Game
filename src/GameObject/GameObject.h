#pragma once

#include "../utils/Vector2D.h"

class GameObject
{
public:
    struct Config {
        Vector2D position = {0.0f, 0.0f};
        Vector2D size     = {1.0f, 1.0f};
    };
    explicit GameObject(const Config& config);
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