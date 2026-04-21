#include "AttackObject.h"

#include "../ColliderManager/ColliderManager.h"
#include "../TextureManager/TextureManager.h"

AttackObject::AttackObject(const Config &config)
    : DynamicObject(config.dynamicObject),
      attackDamage(config.attackDamage),
      isAttacking(config.isAttacking)
{
  size = {1.0f, 1.0f};
}

void AttackObject::handleInput() {}

void AttackObject::update(float deltaTime)
{
  DynamicObject::update(deltaTime);
}

void AttackObject::draw()
{
  PhysicalObject::draw();
}

float AttackObject::getAttackDamage() const
{
  return attackDamage;
}

void AttackObject::onCollision(const PhysicalObject &otherObject, const Vector2D &overlap)
{
}