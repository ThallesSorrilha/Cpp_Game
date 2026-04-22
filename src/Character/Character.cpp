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
  maxHp = 5;
  currentHp = maxHp;
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