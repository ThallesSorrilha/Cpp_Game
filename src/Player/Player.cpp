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
void Player::draw()
{
  /*SDL_Rect r{350, 250, 100, 100};
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  SDL_RenderFillRect(renderer, &r);*/
}
void Player::shutdown() {}
