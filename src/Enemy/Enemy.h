#pragma once
#include "../Character/Character.h"

class Enemy : public Character
{
public:
    struct Config
    {
        Character::Config character;
    };
    explicit Enemy(const Config &config);

    void handleInput() override;
    void update(float deltaTime) override;
    void draw() override;

private:
    float timeWalking = 0.0f;
    float walking = 0.0f;
};