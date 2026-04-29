#pragma once

#include "Character.h"
#include "AttackObject.h"

class Player : public Character
{
public:
    struct Config
    {
        Character::Config character;
        int coins = 0;
    };
    explicit Player(const Config &config);

    void handleInput() override;
    void update(float deltaTime) override;
    void draw() override;
    void onCollision(const PhysicalObject &otherObject) override;
    std::unique_ptr<AttackObject> createAttack() override;

private:
    int coins;
    bool wasAttackKeyDown = false;
    Vector2D getAttackDirection() const;
};