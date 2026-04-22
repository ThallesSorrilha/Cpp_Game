#include "Character.h"

#include "../ColliderManager/ColliderManager.h"
#include "../TextureManager/TextureManager.h"

Character::Character(const Config &config)
    : DynamicObject(config.dynamicObject),
      maxHp(config.maxHp),
      currentHp(config.currentHp),
      isInvincible(config.isInvincible),
      attackDamage(config.attackDamage),
      isAttacking(config.isAttacking),
      isSufferingDamage(config.isSufferingDamage),
      facing(config.facing)
{
  attackDamage = 1;
}

void Character::handleInput() {}

void Character::update(float deltaTime)
{
  force = inputDirection;

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

bool Character::consumeAttackRequest(AttackRequest &outAttackRequest)
{
  if (!hasPendingAttackRequest)
  {
    return false;
  }

  outAttackRequest = pendingAttackRequest;
  hasPendingAttackRequest = false;
  return true;
}

void Character::receiveDamage(int damage)
{
  currentHp -= damage;
}

void Character::doKnockBack(const ColliderBox &otherColliderBox, float deslocation)
{
  Vector2D direction = ColliderManager::calculateDirectionBetweenObjects(*getColliderBox(), otherColliderBox);
  position += direction * deslocation;
}