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
  addState(CharacterState::Idle);
}

void Character::handleInput() {}

CharacterState Character::getState() const
{
  return this->characterFinalState;
}

std::uint32_t Character::getCharacterStates() const
{
  return this->characterStates;
}

/*
//?
Pensar se os métodos cans devem ser validados pelo estado final ou estados individuais.
*/

bool Character::canProcessInput() const
{
  return !hasState(CharacterState::Dead) && !hasState(CharacterState::Absent) && !hasState(CharacterState::Suffering_Damage) && !hasState(CharacterState::Attacking);
}

bool Character::canMove() const
{
  return canProcessInput();
}

bool Character::canAttack() const
{
  return canProcessInput();
}

bool Character::canReceiveDamage() const
{
  return !hasState(CharacterState::Dead) && !hasState(CharacterState::Absent) && !hasState(CharacterState::Suffering_Damage);
}

void Character::updateMovementFromInput()
{
  if (!this->canMove())
  {
    this->inputDirection = {0.0f, 0.0f};
    resetWalkingTimer();
    return;
  }

  if (inputDirection.x != 0 || inputDirection.y != 0)
  {
    setWalkingTimer(0.2f);

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
    resetWalkingTimer();
  }

  force += inputDirection * maxInputForce;
}

void Character::updateState()
{
  if (!this->isAlive())
  {
    if (!this->isExist())
    {
      clearStates();
      addState(CharacterState::Absent);
      characterFinalState = CharacterState::Absent;
      return;
    }
    clearStates();
    addState(CharacterState::Dead);
    characterFinalState = CharacterState::Dead;
    return;
  }

  if (damageTimer.isIn())
    addState(CharacterState::Suffering_Damage);
  else
    removeState(CharacterState::Suffering_Damage);
  if (attackTimer.isIn())
    addState(CharacterState::Attacking);
  else
    removeState(CharacterState::Attacking);
  if (walkingTimer.isIn())
    addState(CharacterState::Walking);
  else
    removeState(CharacterState::Walking);

  removeState(CharacterState::Idle);
  if (!hasState(CharacterState::Walking) && !hasState(CharacterState::Attacking) && !hasState(CharacterState::Suffering_Damage))
  {
    addState(CharacterState::Idle);
  }

  if (hasState(CharacterState::Suffering_Damage))
    characterFinalState = CharacterState::Suffering_Damage;
  else if (hasState(CharacterState::Attacking))
    characterFinalState = CharacterState::Attacking;
  else if (hasState(CharacterState::Walking))
    characterFinalState = CharacterState::Walking;
  else
    characterFinalState = CharacterState::Idle;
}

void Character::updateAnimationForCurrentState()
{
  AnimationID animID;
  if (characterFinalState == CharacterState::Dead || characterFinalState == CharacterState::Absent)
  {
    animID = AnimationID::Character_Dead;
  }
  else
  {
    animID = static_cast<AnimationID>(std::to_underlying(CharacterToAnimationArray[std::to_underlying(characterFinalState)]) + std::to_underlying(facing));
  }

  swapAnimation(animID);
  updateCurrentAnimation();
}

void Character::clearTimers()
{
  if (this->dyingTimer.isEndExclusive())
  {
    this->exist = false;
    clearStates();
    addState(CharacterState::Absent);
    characterFinalState = CharacterState::Absent;
  }

  if (!attackTimer.isIn())
    resetAttackTimer();
  if (!damageTimer.isIn())
    resetDamageTimer();
  if (!walkingTimer.isIn())
    resetWalkingTimer();
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
    clearStates();
    addState(CharacterState::Dead);
    characterFinalState = CharacterState::Dead;
    return;
  }

  setDamageTimer(0.2f);
  characterFinalState = CharacterState::Suffering_Damage;
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
  if (hasState(CharacterState::Dead))
  {
    if (!dyingTimer.isIn())
    {
      setDyingTimer(this->dieTime);
    }

    this->inputDirection = {0.0f, 0.0f};
    resetWalkingTimer();
    updateAnimationForCurrentState();
    return;
  }
}

bool Character::hasState(CharacterState characterState) const
{
  return (characterStates & (1u << static_cast<std::uint32_t>(characterState))) != 0;
}

void Character::addState(CharacterState characterState)
{
  characterStates |= (1u << static_cast<std::uint32_t>(characterState));
}

void Character::removeState(CharacterState characterState)
{
  characterStates &= ~(1u << static_cast<std::uint32_t>(characterState));
}

void Character::clearStates()
{
  characterStates = 0;
}

void Character::setAttackTimer(float seconds)
{
  attackTimer.setTimer(seconds);
  addState(CharacterState::Attacking);
}

void Character::resetAttackTimer()
{
  attackTimer.reset();
  removeState(CharacterState::Attacking);
}

void Character::setDamageTimer(float seconds)
{
  damageTimer.setTimer(seconds);
  addState(CharacterState::Suffering_Damage);
}

void Character::resetDamageTimer()
{
  damageTimer.reset();
  removeState(CharacterState::Suffering_Damage);
}

void Character::setWalkingTimer(float seconds)
{
  walkingTimer.setTimer(seconds);
  removeState(CharacterState::Idle);
  addState(CharacterState::Walking);
}

void Character::resetWalkingTimer()
{
  walkingTimer.reset();
  removeState(CharacterState::Walking);
}

void Character::setDyingTimer(float seconds)
{
  dyingTimer.setTimer(seconds);
  clearStates();
  addState(CharacterState::Dead);
  characterFinalState = CharacterState::Dead;
}