#include "Character.h"

#include "../ColliderManager/ColliderManager.h"
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
  position += velocity * deltaTime;
  syncColliderToPosition();

  const TileMap *collisionMap = getCollisionMap();
  ColliderBox *colliderBox = getColliderBox();

  if (collisionMap != nullptr && colliderBox != nullptr && colliderBox->isEnabled())
  {
    ColliderManager::resolveMovementAgainstTileMap(position, *colliderBox, *collisionMap);
  }

}

void Character::draw() {}
