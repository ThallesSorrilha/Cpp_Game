#include "../include/Player.h"

#include <iostream>
#include <utility>

#include "../include/TextureManager.h"
#include "../include/definitions/Definitions.h"
#include "../include/enums/LayerID.h"
#include "../include/AudioManager.h"
#include "../include/enums/Audio.h"
#include "../include/Item.h"

Player::Player(const Config &config)
    : Character(config.character),
      coins(config.coins)
{
  maxInputForce = 120.0f;
  colliderBox->setCollisionLayer(LayerUtils::toMask(LayerID::Player));
  colliderBox->setCollisionMask(LayerUtils::toMask(LayerID::World) | LayerUtils::toMask(LayerID::Enemy) | LayerUtils::toMask(LayerID::EnemyAttack) | LayerUtils::toMask(LayerID::Item));
  maxHp = 10;
  currentHp = maxHp;
}

void Player::handleInput()
{
  inputDirection = {0, 0};

  if (!canAttack())
  {
    return;
  }

  if (!canProcessInput())
  {
    return;
  }

  const Uint8 *keyStates = SDL_GetKeyboardState(NULL);

  this->isAttackKeyDown = keyStates[SDL_SCANCODE_J];

  if (this->isAttackKeyDown && (!this->wasAttackKeyDown))
  {
    attackTimer.setTimer(0.2f);
    this->wasAttackKeyDown = isAttackKeyDown;
    return;
  }
  this->wasAttackKeyDown = isAttackKeyDown;

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
  }
}

void Player::update(float deltaTime)
{
  hasPendingObjToCreate = this->attackTimer.isIn();
  Character::update(deltaTime);
}

void Player::draw()
{
  Character::draw();
}

int Player::getCoins() const
{
  return coins;
}

void Player::onCollision(const PhysicalObject &otherObject)
{
  LayerID objType = LayerUtils::getLayer(otherObject.getColliderBox()->getCollisionLayer());

  switch (objType)
  {
  case LayerID::Item:
    if (const auto *item = dynamic_cast<const Item *>(&otherObject))
    {
      ++coins;
      item->collect();
      AudioManager::playSound(Audio::Sound_Coin);
    }
    break;

  case LayerID::Enemy:
    if (const auto *enemy = dynamic_cast<const Character *>(&otherObject))
    {
      if (damageTimer.isIn())
      {
        return;
      }
      damageTimer.setTimer(2.0f);
      AudioManager::playSound(Audio::Sound_Impact);
      receiveDamage(enemy->getAttackDamage());
      doKnockBack(*enemy->getColliderBox());
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

std::unique_ptr<AttackObject> Player::createAttack()
{
  Vector2D kAttackSize = {0.0f, 0.0f};
  Vector2D spawnTopLeft = {0.0f, 0.0f};
  Vector2D attackOffset = {0.0f, 0.0f};
  AnimationID animationID = AnimationID::Attack_Down;
  constexpr std::uint32_t collisionLayer = LayerUtils::toMask(LayerID::PlayerAttack);
  constexpr float timeExist = 0.2;

  switch (facing)
  {
  case Facing::Down:
    kAttackSize = {3.0f, 2.0f};
    attackOffset = {-1.0f, 0.0f};
    animationID = AnimationID::Attack_Down;
    break;
  case Facing::Up:
    kAttackSize = {3.0f, 2.0f};
    attackOffset = {-1.0f, -1.0f};
    animationID = AnimationID::Attack_Up;
    break;
  case Facing::Left:
    kAttackSize = {2.0f, 3.0f};
    attackOffset = {-1.0f, -1.0f};
    animationID = AnimationID::Attack_Left;
    break;
  case Facing::Right:
    kAttackSize = {2.0f, 3.0f};
    attackOffset = {0.0f, -1.0f};
    animationID = AnimationID::Attack_Right;
    break;

  default:
    break;
  }

  spawnTopLeft = {position.x + attackOffset.x, position.y + attackOffset.y};

  auto attackObject = std::make_unique<AttackObject>(AttackObject::Config{
      .dynamicObject = {
          .physicalObject = {
              .gameObject = {
                  .position = spawnTopLeft,
                  .size = kAttackSize,
                  .spriteID = SpriteID::Attack},
              .colliderBox = {.collisionLayer = collisionLayer, .collisionMask = 0, .offset = {0.0f, 0.0f}, .size = kAttackSize}}},
      .attackDamage = getAttackDamage(),
      .isAttacking = true,
      .timeExist = timeExist,
      .targetPosition = &position,
      .deslocation = attackOffset,
      .animationID = animationID,
      .animationPosition = Vector2D{-1.0f, -1.0f} + position,
      .animationSize = {3.0f, 3.0f}});

  hasPendingObjToCreate = false;

  return attackObject;
}
