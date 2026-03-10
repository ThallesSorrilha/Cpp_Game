#include "Enemy.h"

Enemy::Enemy(const Config &config)
    : Character(config.character)
{
}

bool Enemy::init() { return true; }
void Enemy::handleInput() {}
void Enemy::update() {}
void Enemy::draw() {}
void Enemy::shutdown() {}
