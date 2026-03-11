#include "Player.h"

Player::Player(const Config &config)
    : Character(config.character),
      inputDirection(config.inputDirection),
      coins(config.coins)
{
}

bool Player::init() { return true; }
void Player::handleInput() {}
void Player::update(float deltaTime) {}
void Player::draw() {}
void Player::shutdown() {}
