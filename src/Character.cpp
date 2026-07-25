#include "../include/Character.h"

#include "../include/ColliderManager.h"
#include "../include/TextureManager.h"
#include "../include/enums/AnimationID.h"

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
  animationsIDs = {
      AnimationID::Character_WalkDown,
      AnimationID::Character_WalkUp,
      AnimationID::Character_WalkLeft,
      AnimationID::Character_WalkRight,
      AnimationID::Character_AttackDown,
      AnimationID::Character_AttackUp,
      AnimationID::Character_AttackLeft,
      AnimationID::Character_AttackRight};

  animations.reserve(animationsIDs.size());
  for (auto animationID : animationsIDs)
  {
    animations.emplace_back(this->texture, animationID);
  }
}

void Character::handleInput() {}

void Character::update(float deltaTime)
{
  if (inputDirection.x != 0 || inputDirection.y != 0)
  {
    if (!(inputDirection.x != 0 && inputDirection.y != 0))
    {
      if (inputDirection.x != 0)
      {
        if (inputDirection.x > 0)
        {
          facing = Facing::Right;
        }
        else if (inputDirection.x < 0)
        {
          facing = Facing::Left;
        }
      }
      else if (inputDirection.y != 0)
      {
        if (inputDirection.y > 0)
        {
          facing = Facing::Down;
        }
        else if (inputDirection.y < 0)
        {
          facing = Facing::Up;
        }
      }
    }
  }

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

  switch (facing)
  {
  case Facing::Down:
    setCurrentAnimation(0);
    break;
  case Facing::Up:
    setCurrentAnimation(1);
    break;
  case Facing::Left:
    setCurrentAnimation(2);
    break;
  case Facing::Right:
    setCurrentAnimation(3);
    break;

  default:
    break;
  }
}

void Character::draw()
{
  PhysicalObject::draw();
}

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