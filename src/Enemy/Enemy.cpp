#include "Enemy.h"

#include <cmath>
#include <random>

#include "../TextureManager/TextureManager.h"
#include "../definitions/Definitions.h"
#include "../enums/LayerID.h"

Enemy::Enemy(const Config &config)
    : Character(config.character)
{
    maxSpeed = 1.5f;
    colliderBox->setCollisionLayer(toMask(LayerID::Enemy));
    colliderBox->setCollisionMask(toMask(LayerID::World));
}

void Enemy::handleInput()
{
}

void Enemy::update(float deltaTime)
{
    stroll(deltaTime);
    Character::update(deltaTime);
}

void Enemy::draw()
{
    PhysicalObject::draw();
}

void Enemy::stroll(float deltaTime)
{
    walking += deltaTime;
    if (walking >= timeWalking)
    {
        timeWalking = walkTimeDist(rng);
        walking = 0.0f;
        if (idleChance(rng))
        {
            moveDirection = {0.0f, 0.0f};
            return;
        }
        const float angle = angleDist(rng);
        moveDirection = {std::cos(angle), std::sin(angle)};
    }
}
