#include "../include/Player.h"

#include <iostream>
#include <utility>

#include "../include/TextureManager.h"
#include "../include/definitions/Definitions.h"
#include "../include/enums/LayerID.h"

Player::Player(const Config &config)
    : Character(config.character),
      coins(config.coins)
{
  maxSpeed = 3.0f;
  colliderBox->setCollisionLayer(LayerUtils::toMask(LayerID::Player));
  colliderBox->setCollisionMask(LayerUtils::toMask(LayerID::World) | LayerUtils::toMask(LayerID::Enemy) | LayerUtils::toMask(LayerID::EnemyAttack));
  maxHp = 5;
  currentHp = maxHp;
}

void Player::handleInput()
{
  isAttacking = false;
  inputDirection = {0, 0};
  const Uint8 *keyStates = SDL_GetKeyboardState(NULL);
  const bool isAttackKeyDown = keyStates[SDL_SCANCODE_J] != 0;

  if (attackTimer.isIn())
  {
    return;
  }
  if (isAttackKeyDown && !wasAttackKeyDown)
  {
    attackTimer.setTimer(0.2f);
    isAttacking = true;
    wasAttackKeyDown = isAttackKeyDown;
    return;
  }
  wasAttackKeyDown = isAttackKeyDown;

  if (keyStates[SDL_SCANCODE_A])
    inputDirection.x += -1.0f;
  if (keyStates[SDL_SCANCODE_W])
    inputDirection.y += -1.0f;
  if (keyStates[SDL_SCANCODE_S])
    inputDirection.y += 1.0f;
  if (keyStates[SDL_SCANCODE_D])
    inputDirection.x += 1.0f;

  if (inputDirection.x != 0 || inputDirection.y != 0)
  {
    inputDirection.normalize();

    if (std::abs(inputDirection.x) > std::abs(inputDirection.y))
    {
      facing = (inputDirection.x > 0.0f) ? Facing::Right : Facing::Left;
    }
    else
    {
      facing = (inputDirection.y > 0.0f) ? Facing::Down : Facing::Up;
    }
  }
}

void Player::update(float deltaTime)
{
  if (isAttacking)
  {
    pendingAttackRequest = AttackRequest{
        .position = position,
        .direction = getAttackDirection(),
        .damage = getAttackDamage(),
        .collisionLayer = LayerUtils::toMask(LayerID::PlayerAttack),
        .collisionMask = 0,
        .timeAlive = 0.2,
    };
    hasPendingAttackRequest = true;
  }

  Character::update(deltaTime);
}

void Player::draw()
{
  PhysicalObject::draw();
}

void Player::onCollision(const PhysicalObject &otherObject)
{
  LayerID objType = LayerUtils::getLayer(otherObject.getColliderBox()->getCollisionLayer());

  switch (objType)
  {
  case LayerID::Enemy:
    if (const auto enemy = dynamic_cast<const Character *>(&otherObject))
    {
      if (damageTimer.isIn())
      {
        return;
      }
      damageTimer.setTimer(2.0f);

      receiveDamage(enemy->getAttackDamage());
      doKnockBack(*enemy->getColliderBox(), 0.5f);
    }
    break;

  default:
    break;
  }
}

Vector2D Player::getAttackDirection() const
{
  if (inputDirection.x != 0.0f || inputDirection.y != 0.0f)
  {
    return inputDirection;
  }

  switch (facing)
  {
  case Facing::Up:
    return {0.0f, -1.0f};
  case Facing::Down:
    return {0.0f, 1.0f};
  case Facing::Left:
    return {-1.0f, 0.0f};
  case Facing::Right:
    return {1.0f, 0.0f};
  default:
    return {0.0f, 1.0f};
  }
}
