#include "Enemy.h"
#include "../TextureManager/TextureManager.h"
#include "../utils/Definitions.h"
#include <random>

Enemy::Enemy(const Config &config)
    : Character(config.character)
{
    maxSpeed = 1.5f;
}

void Enemy::handleInput()
{
}

void Enemy::update(float deltaTime)
{
    walking += deltaTime;
    if (walking > timeWalking)
    {
        timeWalking = (static_cast<float>(rand() % 2001)) / 1000.0f + 1.0f;
        walking = 0.0f;
        float x = (static_cast<float>(rand() % 2001)) / 1000.0f - 1.0f;
        float y = (static_cast<float>(rand() % 2001)) / 1000.0f - 1.0f;
        moveDirection = {x, y};
        moveDirection.conditional_normalize(1);
        if (moveDirection.length() < 0.7f)
        {
            moveDirection = {0.0f, 0.0f};
        }
    }
    Character::update(deltaTime);
}

void Enemy::draw()
{
    DynamicObject::draw();
}
