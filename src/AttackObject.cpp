#include "../include/AttackObject.h"

#include "../include/ColliderManager.h"
#include "../include/TextureManager.h"

AttackObject::AttackObject(const Config &config)
    : DynamicObject(config.dynamicObject),
      attackDamage(config.attackDamage),
      isAttacking(config.isAttacking),
      timeAlive(config.timeAlive),
      deslocation(config.deslocation),
      targetPosition(config.targetPosition)
{
  timerAlive.setTimer(timeAlive);
}

void AttackObject::handleInput() {}

void AttackObject::update(float deltaTime)
{
  (void)deltaTime;
  if (targetPosition == nullptr)
  {
    alive = false;
    return;
  }
  if (timerAlive.isEnd())
  {
    alive = false;
  }
  followObject();
}

void AttackObject::draw()
{
  PhysicalObject::draw();
}

float AttackObject::getAttackDamage() const
{
  return attackDamage;
}

void AttackObject::onCollision(const PhysicalObject &otherObject)
{
  (void)otherObject;
}

void AttackObject::followObject()
{
  position.x = targetPosition->x + deslocation.x;
  position.y = targetPosition->y + deslocation.y;
}