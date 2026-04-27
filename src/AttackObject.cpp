#include "../include/AttackObject.h"

#include "../include/ColliderManager.h"
#include "../include/TextureManager.h"

AttackObject::AttackObject(const Config &config)
    : DynamicObject(config.dynamicObject),
      attackDamage(config.attackDamage),
      isAttacking(config.isAttacking),
      timeAlive(config.timeAlive)
{
  timerAlive.setTimer(timeAlive);
}

void AttackObject::handleInput() {}

void AttackObject::update(float deltaTime)
{
  if (timerAlive.isEnd())
  {
    alive = false;
  }
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

void AttackObject::onCollision(const PhysicalObject &otherObject) {}