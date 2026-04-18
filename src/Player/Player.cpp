#include "Player.h"

#include <iostream>

#include "../TextureManager/TextureManager.h"
#include "../definitions/Definitions.h"
#include "../enums/LayerID.h"

Player::Player(const Config &config)
    : Character(config.character),
      inputDirection(config.inputDirection),
      coins(config.coins)
{
  maxSpeed = 3.0f;
  colliderBox->setCollisionLayer(toMask(LayerID::Player));
  colliderBox->setCollisionMask(toMask(LayerID::World) | toMask(LayerID::Enemy));
}

void Player::handleInput()
{
  inputDirection = {0, 0};
  const Uint8 *keyStates = SDL_GetKeyboardState(NULL);
  if (keyStates[SDL_SCANCODE_A])
    inputDirection.x += -1.0f;
  if (keyStates[SDL_SCANCODE_W])
    inputDirection.y += -1.0f;
  if (keyStates[SDL_SCANCODE_S])
    inputDirection.y += +1.0f;
  if (keyStates[SDL_SCANCODE_D])
    inputDirection.x += 1.0f;

  if (inputDirection.x != 0 || inputDirection.y != 0)
    inputDirection.normalize();
}

void Player::update(float deltaTime)
{
  moveDirection = inputDirection;
  Character::update(deltaTime);
}

void Player::draw()
{
  DynamicObject::draw();
}
