#pragma once
#include "DynamicObject/DynamicObject.h"
#include "enums/Facing.h"

class Character : public DynamicObject
{
public:
    bool init() override;
    void handleInput() override;
    void update() override;
    void draw() override;
    void shutdown() override;

protected:
    int maxHp;
    int currentHp;
    bool isDead = false;
    bool isInvincible = false;
    float attackDamage;
    bool isAttacking = false;
    bool isSufferingDamage = false;
    Facing facing = Facing::Down;
};