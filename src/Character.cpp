#include "../include/Character.h"

#include "../include/ColliderManager.h"
#include "../include/TextureManager.h"

Character::Character(const Config &config)
    : DynamicObject(config.dynamicObject),
      maxHp(config.maxHp),
      currentHp(config.currentHp),
      isInvincible(config.isInvincible),
      attackDamage(config.attackDamage),
      isAttacking(config.isAttacking),
      isSufferingDamage(config.isSufferingDamage),
      facing(config.facing),
      inputDirection(config.inputDirection),
      maxInputForce(config.maxInputForce)
{
  attackDamage = 1;
}

void Character::handleInput() {}

void Character::update(float deltaTime)
{
  force += inputDirection * maxInputForce;

  DynamicObject::update(deltaTime);

  const TileMap *collisionMap = getCollisionMap();
  ColliderBox *colliderBox = getColliderBox();
  if (collisionMap != nullptr && colliderBox != nullptr && colliderBox->isEnabled())
  {
    ColliderManager::resolveMovementAgainstTileMap(position, *colliderBox, *collisionMap);
  }

  if (currentHp <= 0)
  {
    alive = false;
  }
}

void Character::draw() {}

int Character::getAttackDamage() const
{
  return attackDamage;
}

void Character::receiveDamage(int damage)
{
  currentHp -= damage;
}

void Character::doKnockBack(const ColliderBox &otherColliderBox)
{
  Vector2D direction = ColliderManager::calculateDirectionBetweenObjects(*getColliderBox(), otherColliderBox);
  force += (direction * 1000.0f);
}