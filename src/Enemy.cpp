#include "../include/Enemy.h"

#include <cmath>
#include <random>

#include "../include/TextureManager.h"
#include "../include/definitions/Definitions.h"
#include "../include/enums/LayerID.h"
#include "../include/AttackObject.h"

Enemy::Enemy(const Config &config)
    : Character(config.character)
{
    maxInputForce = 80.0f;
    colliderBox->setCollisionLayer(LayerUtils::toMask(LayerID::Enemy));
    colliderBox->setCollisionMask(LayerUtils::toMask(LayerID::World) | LayerUtils::toMask(LayerID::PlayerAttack));
    maxHp = 5;
    currentHp = maxHp;
}

void Enemy::handleInput()
{
}

void Enemy::update(float deltaTime)
{
    if (damageTimer.isEnd())
    {
        stroll(deltaTime);
    }

    Character::update(deltaTime);
}

void Enemy::draw()
{
    Character::draw();
}

void Enemy::stroll(float deltaTime)
{
    if (walkingTimer.isEnd())
    {
        walkingTimer.setTimer(walkTimeDist(rng));
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
            inputDirection = {0.0f, 0.0f};
            walkingTimer.reset();
            receiveDamage(attack->getAttackDamage());
            doKnockBack(*attack->getColliderBox());
        }
        break;

    default:
        break;
    }
}

std::unique_ptr<AttackObject> Enemy::createAttack()
{
    return nullptr;
}