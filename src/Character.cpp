#include "../include/Character.h"

#include "../include/ColliderManager.h"
#include "../include/enums/AnimationID.h"
#include "../include/definitions/CharacterToAnimationArray.h"

#include <algorithm>
#include <utility>

Character::Character(const Config &config)
    : DynamicObject(config.dynamicObject),
      maxHp(config.maxHp),
      currentHp(config.currentHp),
      attackDamage(config.attackDamage),
      facing(config.facing),
      inputDirection(config.inputDirection),
      maxInputForce(config.maxInputForce)
{
  attackDamage = 1;
  initializeAnimations(AnimationSet::Character);

  this->lastUniqueDirection = facing;
  this->state = CharacterState::Idle;
}

void Character::handleInput() {}

CharacterState Character::getState() const
{
  return this->state;
}

/*
//?
Pensar se os métodos cans devem ser validados pelo estado final ou estados individuais.
*/

bool Character::canProcessInput() const
{
  return this->state != CharacterState::Dead && this->state != CharacterState::Suffering_Damage && this->state != CharacterState::Attacking;
}

bool Character::canMove() const
{
  return this->state != CharacterState::Dead;
}

bool Character::canAttack() const
{
  return this->state != CharacterState::Dead && this->state != CharacterState::Suffering_Damage && this->state != CharacterState::Attacking;
}

bool Character::canReceiveDamage() const
{
  return this->state != CharacterState::Dead && this->state != CharacterState::Suffering_Damage;
}

void Character::updateMovementFromInput()
{
  if (!this->canMove())
  {
    this->inputDirection = {0.0f, 0.0f};
    this->walkingTimer.reset();
    return;
  }

  if (inputDirection.x != 0 || inputDirection.y != 0)
  {
    this->walkingTimer.setTimer(0.2f);

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
    else
    {
      this->facing = this->lastUniqueDirection;
    }
    this->lastUniqueDirection = this->facing;
  }
  else
  {
    walkingTimer.reset();
  }

  force += inputDirection * maxInputForce;
}

void Character::updateState()
{
  if (!this->isAlive())
  {
    if (!this->isExist())
    {
      state = CharacterState::Absent;
      return;
    }
    state = CharacterState::Dead;
    return;
  }

  if (this->damageTimer.isIn())
  {
    state = CharacterState::Suffering_Damage;
    return;
  }

  if (this->attackTimer.isIn())
  {
    state = CharacterState::Attacking;
    return;
  }

  if (this->walkingTimer.isIn())
  {
    state = CharacterState::Walking;
    return;
  }

  state = CharacterState::Idle;
}

void Character::updateAnimationForCurrentState()
{
  AnimationID animID;
  if (state == CharacterState::Dead)
  {
    animID = AnimationID::Character_Dead;
  }
  else
  {
    animID = static_cast<AnimationID>(std::to_underlying(CharacterToAnimationArray[std::to_underlying(state)]) + std::to_underlying(facing));
  }

  swapAnimation(animID);
  updateCurrentAnimation();
}

void Character::clearTimers()
{
  if (this->dyingTimer.isEndExclusive())
  {
    this->exist = false;
  }
}

void Character::update(float deltaTime)
{
  /*
  Atualização de CharacterState e timers
  Atualização de entradas
  Atualização da física
  Verificação de colisão
  Atualização de animação
  */

  clearTimers();
  updateState();

  this->deathManage();

  updateMovementFromInput();

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

  updateAnimationForCurrentState();
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
  if (!this->canReceiveDamage())
  {
    return;
  }

  this->currentHp -= damage;
  if (this->currentHp <= 0)
  {
    this->currentHp = 0;
    this->alive = false;
    this->state = CharacterState::Dead;
    return;
  }

  this->damageTimer.setTimer(0.2f);
  this->state = CharacterState::Suffering_Damage;
}

void Character::doKnockBack(const ColliderBox &otherColliderBox)
{
  Vector2D direction = ColliderManager::calculateDirectionBetweenObjects(*getColliderBox(), otherColliderBox);
  force += (direction * 1500.0f);
}

bool Character::isAlive() const
{
  return this->alive;
}

void Character::deathManage()
{
  if (this->state == CharacterState::Dead)
  {
    if (!dyingTimer.isIn())
    {
      this->dyingTimer.setTimer(this->dieTime);
    }

    this->inputDirection = {0.0f, 0.0f};
    this->walkingTimer.reset();
    updateAnimationForCurrentState();
    return;
  }
}