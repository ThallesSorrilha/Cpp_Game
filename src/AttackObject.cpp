#include "../include/AttackObject.h"

#include "../include/ColliderManager.h"
#include "../include/TextureManager.h"

AttackObject::AttackObject(const Config &config)
    : DynamicObject(config.dynamicObject),
      attackDamage(config.attackDamage),
      isAttacking(config.isAttacking),
      timeExist(config.timeExist),
      deslocation(config.deslocation),
      targetPosition(config.targetPosition),
      animationID(config.animationID),
      animationPosition(config.animationPosition),
      animationSize(config.animationSize)
{
  initializeAnimations(AnimationSet::Attack);
  swapAnimation(config.animationID);

  if (targetPosition != nullptr)
  {
    animationOffset.x = animationPosition.x - targetPosition->x;
    animationOffset.y = animationPosition.y - targetPosition->y;
  }

  timerExist.setTimer(timeExist);
}

void AttackObject::handleInput() {}

void AttackObject::update(float deltaTime)
{
  (void)deltaTime;
  if (targetPosition == nullptr)
  {
    exist = false;
    return;
  }
  if (timerExist.isEnd())
  {
    exist = false;
  }
  followObject();

  updateCurrentAnimation();
}

void AttackObject::draw()
{
  Animation *currentAnimation = getCurrentAnimation();
  if (currentAnimation != nullptr)
  {
    currentAnimation->draw(animationPosition, animationSize);
  }
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

  animationPosition.x = targetPosition->x + animationOffset.x;
  animationPosition.y = targetPosition->y + animationOffset.y;
}