#pragma once

#include <random>

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
    void onCollision(const PhysicalObject &otherObject, const Vector2D &overlap) override;

    void stroll(float deltaTime);

private:
    float timeWalking = 0.0f;
    float walking = 0.0f;
    std::mt19937 rng = std::mt19937(std::random_device{}());
    std::uniform_real_distribution<float> walkTimeDist = std::uniform_real_distribution<float>(1.0f, 3.0f);
    std::uniform_real_distribution<float> angleDist = std::uniform_real_distribution<float>(0.0f, 6.28318530718f);
    std::bernoulli_distribution idleChance = std::bernoulli_distribution(0.4);
};