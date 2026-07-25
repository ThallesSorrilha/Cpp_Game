#include "../include/AttackObject.h"

#include "../include/ColliderManager.h"
#include "../include/TextureManager.h"

AttackObject::AttackObject(const Config &config)
    : DynamicObject(config.dynamicObject),
      attackDamage(config.attackDamage),
      isAttacking(config.isAttacking),
      timeAlive(config.timeAlive),
      deslocation(config.deslocation),
      targetPosition(config.targetPosition),
      animationID(config.animationID),
      animationPosition(config.animationPosition),
      animationSize(config.animationSize)
{
  animationsIDs = {config.animationID};
  animations.reserve(animationsIDs.size());
  for (auto animationID : animationsIDs)
  {
    animations.emplace_back(this->texture, animationID);
  }

  if (targetPosition != nullptr)
  {
    animationOffset.x = animationPosition.x - targetPosition->x;
    animationOffset.y = animationPosition.y - targetPosition->y;
  }

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