#include "../include/Character.h"

#include "../include/ColliderManager.h"
#include "../include/TextureManager.h"
#include "../include/enums/AnimationID.h"
#include "../include/definitions/CharacterToAnimationArray.h"

#include <algorithm>

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
      AnimationID::Character_IdleDown,
      AnimationID::Character_IdleUp,
      AnimationID::Character_IdleLeft,
      AnimationID::Character_IdleRight,
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
  
  this->lastUniqueDirection = facing;
}

void Character::handleInput() {}

void Character::update(float deltaTime)
{
  if (inputDirection.x != 0 || inputDirection.y != 0)
  {
    isWalking = true;

    if (std::abs(inputDirection.x) > std::abs(inputDirection.y))
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
    else if (std::abs(inputDirection.y) > std::abs(inputDirection.x))
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
    else {
      this->facing = this->lastUniqueDirection;
    }
    this->lastUniqueDirection = this->facing;
  }
  else
  {
    isWalking = false;
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

  if (this->attackTimer.isEnd())
  {
    this->isAttacking = false;
  }
  if (this->damageTimer.isEnd())
  {
    this->isSufferingDamage = false;
  }

  if (!this->isAlive())
  {
    state = CharacterState::Dead;
  }
  else if (this->isSufferingDamage)
  {
    state = CharacterState::Suffering_Damage;
  }
  else if (this->isAttacking)
  {
    state = CharacterState::Attacking;
  }
  else if (this->isWalking)
  {
    state = CharacterState::Walking;
  }
  else
  {
    state = CharacterState::Idle;
  }

  AnimationID animID = static_cast<AnimationID>(std::to_underlying(CharacterToAnimationArray[std::to_underlying(state)]) + std::to_underlying(facing));

  std::size_t animationIndex = this->currentAnimation;
  auto it = std::find(this->animationsIDs.begin(), this->animationsIDs.end(), animID);
  if (it != this->animationsIDs.end())
  {
    animationIndex = static_cast<std::size_t>(std::distance(this->animationsIDs.begin(), it));
  }
  this->setCurrentAnimation(animationIndex);

  this->animations[this->currentAnimation].update();
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
  this->currentHp -= damage;
  isSufferingDamage = true;
}

void Character::doKnockBack(const ColliderBox &otherColliderBox)
{
  Vector2D direction = ColliderManager::calculateDirectionBetweenObjects(*getColliderBox(), otherColliderBox);
  force += (direction * 1000.0f);
}