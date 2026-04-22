#pragma once

#include "../DynamicObject/DynamicObject.h"
#include "../utils/Timer.h"

class AttackObject : public DynamicObject
{
public:
    struct Config
    {
        DynamicObject::Config dynamicObject;
        int attackDamage = 0;
        bool isAttacking = true;
        float timeAlive = 0.0f;
    };
    explicit AttackObject(const Config &config);

    void handleInput() override;
    void update(float deltaTime) override;
    void draw() override;
    float getAttackDamage() const;
    void onCollision(const PhysicalObject &otherObject) override;

protected:
    float attackDamage;
    bool isAttacking;
    float timeAlive;
    Timer timerAlive;
};