#pragma once
#include "GameObject/GameObject.h"

class DynamicObject : public GameObject
{
public:
    bool init() override;
    void handleInput() override;
    void update() override;
    void draw() override;
    void shutdown() override;

protected:
    Vector2D velocity;
    Vector2D acceleration;
    float maxSpeed = 5.0f;
    float friction = 30.0f;
    float mass = 1.0f;
};