#pragma once

#include <memory>

#include "../GameObject/GameObject.h"
#include "../ColliderBox/ColliderBox.h"

class TileMap;

class DynamicObject : public GameObject
{
public:
    struct Config
    {
        GameObject::Config gameObject;
        ColliderBox::Config colliderBox;
        Vector2D velocity = {0.0f, 0.0f};
        Vector2D acceleration = {0.0f, 0.0f};
        float maxSpeed = 5.0f;
        float friction = 30.0f;
        float mass = 1.0f;
    };
    explicit DynamicObject(const Config &config);
    virtual ~DynamicObject() override;

    void handleInput() override;
    void update(float deltaTime) override;
    void draw() override;

    void setCollisionMap(const TileMap *newCollisionMap);

protected:
    void syncColliderToPosition();
    const TileMap *getCollisionMap() const;
    const ColliderBox *getColliderBox() const;

    Vector2D velocity;
    Vector2D acceleration;
    float maxSpeed;
    float friction;
    float mass;
    std::unique_ptr<ColliderBox> colliderBox;
    const TileMap *collisionMap = nullptr;
};