#pragma once

#include "DynamicObject.h"
#include "enums/Facing.h"
#include "structs/AttackRequest.h"
#include "utils/Timer.h"

class Character : public DynamicObject
{
public:
    struct Config
    {
        DynamicObject::Config dynamicObject;
        int maxHp = 0;
        int currentHp = 0;
        bool isInvincible = false;
        int attackDamage = 0;
        bool isAttacking = false;
        bool isSufferingDamage = false;
        Facing facing = Facing::Down;
        Vector2D inputDirection = {0.0f, 0.0f};
    };
    explicit Character(const Config &config);

    void handleInput() override;
    void update(float deltaTime) override;
    void draw() override;
    int getAttackDamage() const;
    bool consumeAttackRequest(AttackRequest &outAttackRequest);
    void onCollision(const PhysicalObject &otherObject) override = 0;
    void receiveDamage(int damage);
    void doKnockBack(const ColliderBox &otherColliderBox, float deslocation);

protected:
    int maxHp;
    int currentHp;
    bool isInvincible;
    int attackDamage;
    bool isAttacking;
    bool isSufferingDamage;
    Facing facing;
    Vector2D inputDirection;
    AttackRequest pendingAttackRequest;
    bool hasPendingAttackRequest = false;
    Timer attackTimer;
    Timer damageTimer;
};