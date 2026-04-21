#include "Player.h"

#include <iostream>
#include <utility>

#include "../TextureManager/TextureManager.h"
#include "../definitions/Definitions.h"
#include "../enums/LayerID.h"

Player::Player(const Config &config)
    : Character(config.character),
      coins(config.coins)
{
  maxSpeed = 3.0f;
  colliderBox->setCollisionLayer(LayerUtils::toMask(LayerID::Player));
  colliderBox->setCollisionMask(LayerUtils::toMask(LayerID::World) | LayerUtils::toMask(LayerID::Enemy));
}

void Player::handleInput()
{
  inputDirection = {0, 0};
  isAttacking = false;
  const Uint8 *keyStates = SDL_GetKeyboardState(NULL);

  if (keyStates[SDL_SCANCODE_J])
  {
    isAttacking = true;
    return;
  }

  if (keyStates[SDL_SCANCODE_A])
    inputDirection.x += -1.0f;
  if (keyStates[SDL_SCANCODE_W])
    inputDirection.y += -1.0f;
  if (keyStates[SDL_SCANCODE_S])
    inputDirection.y += +1.0f;
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
    // criar ataque com base no Facing
    // inserir em GameWorld::physicObjects
  }
  Character::update(deltaTime);
}

void Player::draw()
{
  PhysicalObject::draw();
}

void Player::onCollision(const PhysicalObject &otherObject, const Vector2D &overlap)
{
  LayerID objType = LayerUtils::getLayer(otherObject.getColliderBox()->getCollisionLayer());

  switch (objType)
  {
  case LayerID::Enemy:
    if (const auto enemy = dynamic_cast<const Character *>(&otherObject))
    {
      int damage = enemy->getAttackDamage();
      currentHp -= damage;
      // std::cout << currentHp << std::endl;
      //  aplicar um timer de invencibilidade no player
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