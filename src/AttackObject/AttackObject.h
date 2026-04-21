#pragma once

#include "../DynamicObject/DynamicObject.h"

class AttackObject : public DynamicObject
{
public:
    struct Config
    {
        DynamicObject::Config dynamicObject;
        int attackDamage = 0;
        bool isAttacking = true;
    };
    explicit AttackObject(const Config &config);

    void handleInput() override;
    void update(float deltaTime) override;
    void draw() override;
    float getAttackDamage() const;
    void onCollision(const PhysicalObject &otherObject, const Vector2D &overlap) override;

protected:
    float attackDamage;
    bool isAttacking;
};