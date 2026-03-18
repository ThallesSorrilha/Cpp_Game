#include "Character.h"

Character::Character(const Config &config)
    : DynamicObject(config.dynamicObject),
      maxHp(config.maxHp),
      currentHp(config.currentHp),
      isDead(config.isDead),
      isInvincible(config.isInvincible),
      attackDamage(config.attackDamage),
      isAttacking(config.isAttacking),
      isSufferingDamage(config.isSufferingDamage),
      facing(config.facing)
{
}

bool Character::init() { return true; }
void Character::handleInput() {}
void Character::update(float deltaTime)
{
  velocity = moveDirection * maxSpeed;
  position += velocity * deltaTime;
}
void Character::draw() {}
void Character::shutdown() {}
