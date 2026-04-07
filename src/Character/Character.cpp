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
  const Vector2D delta = velocity * deltaTime;

  const TileMap *collisionMap = getCollisionMap();
  const ColliderBox *colliderBox = getColliderBox();

  if (collisionMap != nullptr && colliderBox != nullptr && colliderBox->isEnabled())
  {
    position = ColliderManager::resolveMovementAgainstTileMap(position, delta, *colliderBox, *collisionMap);
  }
  else
  {
    position += delta;
  }

  syncColliderToPosition();
}

void Character::draw() {}
