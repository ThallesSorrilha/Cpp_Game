#pragma once
#include "../DynamicObject/DynamicObject.h"
#include "../enums/Facing.h"

class Character : public DynamicObject
{
public:
    struct Config 
    {
        DynamicObject::Config dynamicObject;
        int maxHp = 0;
        int currentHp = 0;
        bool isDead = false;
        bool isInvincible = false;
        float attackDamage = 0;
        bool isAttacking = false;
        bool isSufferingDamage = false;
        Facing facing = Facing::Down;
        Vector2D moveDirection = {0.0f, 0.0f};
    };
    explicit Character(const Config &config);

    bool init() override;
    void handleInput() override;
    void update(float deltaTime) override;
    void draw() override;
    void shutdown() override;

protected:
    int maxHp;
    int currentHp;
    bool isDead;
    bool isInvincible;
    float attackDamage;
    bool isAttacking;
    bool isSufferingDamage;
    Facing facing;
    Vector2D moveDirection;
};