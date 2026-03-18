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
    TextureManager::draw(
        texture,
        std::round(position.x * PIXELS_PER_BLOCK),
        std::round(position.y * PIXELS_PER_BLOCK),
        std::round(size.x * PIXELS_PER_BLOCK),
        std::round(size.y * PIXELS_PER_BLOCK));
}

void Enemy::shutdown()
{
    DynamicObject::shutdown();
}
