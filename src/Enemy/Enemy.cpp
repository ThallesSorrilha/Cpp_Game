#include "Enemy.h"
#include "../TextureManager/TextureManager.h"
#include "../utils/Definitions.h"

Enemy::Enemy(const Config &config)
    : Character(config.character)
{
}

bool Enemy::init()
{
    return DynamicObject::init();
}

void Enemy::handleInput() {}

void Enemy::update(float deltaTime)
{
    moveDirection = {0.0f, 0.0f};
    Character::update(deltaTime);
}

void Enemy::draw()
{
    DynamicObject::draw();
}

void Enemy::shutdown()
{
    DynamicObject::shutdown();
}
