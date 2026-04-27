#pragma once

#include "PhysicalObject.h"
#include "ColliderBox.h"
#include "TileMap.h"

class DynamicObject : public PhysicalObject
{
public:
    struct Config
    {
        PhysicalObject::Config physicalObject;
        Vector2D force = {0.0f, 0.0f};
        Vector2D velocity = {0.0f, 0.0f};
        Vector2D acceleration = {0.0f, 0.0f};
        float maxSpeed = 5.0f;
        float friction = 30.0f;
        float mass = 1.0f;
    };
    explicit DynamicObject(const Config &config);

    void handleInput() override;
    void update(float deltaTime) override;
    void draw() override;
    void onCollision(const PhysicalObject &otherObject) override = 0;

    void setCollisionMap(const TileMap *newCollisionMap);

protected:
    const TileMap *getCollisionMap() const;

    Vector2D force;
    Vector2D velocity;
    Vector2D acceleration;
    float maxSpeed;
    float friction;
    float mass;
    const TileMap *collisionMap = nullptr;
};