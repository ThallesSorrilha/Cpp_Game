#include "Player.h"
#include "../TextureManager/TextureManager.h"
#include "../utils/Definitions.h"

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

void Player::handleInput() {}
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
void Player::shutdown() {}
