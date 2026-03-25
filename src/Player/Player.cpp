#include "Player.h"
#include "../TextureManager/TextureManager.h"
#include "../utils/Definitions.h"
#include <iostream>

Player::Player(const Config &config)
    : Character(config.character),
      inputDirection(config.inputDirection),
      coins(config.coins)
{
  maxSpeed = 3.0f;
}

bool Player::init()
{
  return DynamicObject::init();
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

void Player::shutdown()
{
  DynamicObject::shutdown();
}
