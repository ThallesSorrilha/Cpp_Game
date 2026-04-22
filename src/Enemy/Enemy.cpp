#include "Enemy.h"

#include <cmath>
#include <random>

#include "../TextureManager/TextureManager.h"
#include "../definitions/Definitions.h"
#include "../enums/LayerID.h"
#include "../AttackObject/AttackObject.h"

Enemy::Enemy(const Config &config)
    : Character(config.character)
{
    maxSpeed = 1.5f;
    colliderBox->setCollisionLayer(LayerUtils::toMask(LayerID::Enemy));
    colliderBox->setCollisionMask(LayerUtils::toMask(LayerID::World) | LayerUtils::toMask(LayerID::PlayerAttack));
    maxHp = 2;
    currentHp = maxHp;
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
            inputDirection = {0.0f, 0.0f};
            return;
        }
        const float angle = angleDist(rng);
        inputDirection = {std::cos(angle), std::sin(angle)};
    }
}

void Enemy::onCollision(const PhysicalObject &otherObject)
{
    LayerID objType = LayerUtils::getLayer(otherObject.getColliderBox()->getCollisionLayer());

    switch (objType)
    {
    case LayerID::PlayerAttack:
        if (const auto attack = dynamic_cast<const AttackObject *>(&otherObject))
        {
            if (damageTimer.isIn())
            {
                return;
            }
            damageTimer.setTimer(0.22f);

            receiveDamage(attack->getAttackDamage());
            doKnockBack(*attack->getColliderBox(), 0.2f);
        }
        break;

    default:
        break;
    }
}