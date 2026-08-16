#pragma once

#include "DynamicObject.h"
#include "enums/Facing.h"
#include "AttackObject.h"
#include "utils/Timer.h"
#include "enums/CharacterState.h"

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
        float maxInputForce = 80.0f;
    };
    explicit Character(const Config &config);

    void handleInput() override;
    void update(float deltaTime) override;
    void draw() override;
    int getAttackDamage() const;
    void onCollision(const PhysicalObject &otherObject) override = 0;
    void receiveDamage(int damage);
    void doKnockBack(const ColliderBox &otherColliderBox);
    virtual std::unique_ptr<AttackObject> createAttack() = 0;
    bool isAlive() const;
    void dying();

protected:
    int maxHp;
    int currentHp;
    bool isInvincible;
    int attackDamage;
    bool isWalking;
    bool isAttacking;
    bool isSufferingDamage;
    Facing lastUniqueDirection;
    Facing facing;
    Vector2D inputDirection;
    Timer attackTimer;
    Timer damageTimer;
    float maxInputForce;
    CharacterState state = CharacterState::Idle;
    bool alive = true;
    bool isDying = false;
    Timer dyingTimer;
    float dieTime = 1.0f;
};