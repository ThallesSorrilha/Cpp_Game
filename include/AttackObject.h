#pragma once

#include "DynamicObject.h"
#include "enums/AnimationID.h"
#include "utils/Timer.h"

class AttackObject : public DynamicObject
{
public:
    struct Config
    {
        DynamicObject::Config dynamicObject;
        int attackDamage = 0;
        bool isAttacking = true;
        float timeExist = 0.0f;
        Vector2D *targetPosition = nullptr;
        Vector2D deslocation = {0.0f, 0.0f};
        AnimationID animationID = AnimationID::Attack_Down;
        Vector2D animationPosition = {0.0f, 0.0f};
        Vector2D animationSize = {3.0f, 3.0f};
    };
    explicit AttackObject(const Config &config);

    void handleInput() override;
    void update(float deltaTime) override;
    void draw() override;
    float getAttackDamage() const;
    virtual void onCollision(const PhysicalObject &otherObject) override;

protected:
    float attackDamage;
    bool isAttacking;
    float timeExist;
    Timer timerExist;
    Vector2D deslocation;
    Vector2D *targetPosition;
    AnimationID animationID;
    Vector2D animationPosition;
    Vector2D animationSize;
    Vector2D animationOffset = {0.0f, 0.0f};

    void followObject();
};