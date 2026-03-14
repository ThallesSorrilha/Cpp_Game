#include "Player.h"
#include "../TextureManager/TextureManager.h"
#include "../utils/Definitions.h"
#include <iostream>

Player::Player(const Config &config)
    : Character(config.character),
      inputDirection(config.inputDirection),
      coins(config.coins)
{
}

bool Player::init()
{
  texture = TextureManager::load(textureFilePath);
  if (texture == nullptr)
    return false;
  return true;
}

void Player::handleInput()
{
  inputDirection = {0, 0};
  const Uint8 *keyStates = SDL_GetKeyboardState(NULL);
  if (keyStates[SDL_SCANCODE_A])
    inputDirection.x += -1.0f;
  if (keyStates[SDL_SCANCODE_W])
    inputDirection.y += 1.0f;
  if (keyStates[SDL_SCANCODE_S])
    inputDirection.y += -1.0f;
  if (keyStates[SDL_SCANCODE_D])
    inputDirection.x += 1.0f;
}

void Player::update(float deltaTime) {}

void Player::draw()
{
  TextureManager::draw(
      texture,
      std::round(position.x * PIXELS_PER_BLOCK),
      std::round(position.y * PIXELS_PER_BLOCK),
      std::round(size.x * PIXELS_PER_BLOCK),
      std::round(size.y * PIXELS_PER_BLOCK));
}
void Player::shutdown()
{
  if (texture != nullptr)
  {
    SDL_DestroyTexture(texture);
    texture = nullptr;
  }
}
