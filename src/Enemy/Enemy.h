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

    bool init() override;
    void handleInput() override;
    void update(float deltaTime) override;
    void draw() override;
    void shutdown() override;

private:
    float timeWalking = 0.0f;
    float walking = 0.0f;
    
};