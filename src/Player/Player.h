#pragma once

#include "../Character/Character.h"

class Player : public Character
{
public:
    struct Config
    {
        Character::Config character;
        Vector2D inputDirection = {0, 0};
        int coins = 0;
    };
    explicit Player(const Config &config);

    void handleInput() override;
    void update(float deltaTime) override;
    void draw() override;

private:
    Vector2D inputDirection;
    int coins;
};