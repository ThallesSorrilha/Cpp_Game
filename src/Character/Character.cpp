#include "Character.h"
#include "../TextureManager/TextureManager.h"

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

void Character::handleInput() {}

void Character::update(float deltaTime)
{
  velocity = moveDirection * maxSpeed;

  const Vector2D delta = velocity * deltaTime;
  Vector2D candidatePosition = position;

  candidatePosition.x += delta.x;
  if (!intersectsMapAtPosition(candidatePosition))
  {
    position.x = candidatePosition.x;
  }

  candidatePosition = position;
  candidatePosition.y += delta.y;
  if (!intersectsMapAtPosition(candidatePosition))
  {
    position.y = candidatePosition.y;
  }

  syncColliderToPosition();
}

void Character::draw() {}
