#include "DynamicObject.h"
#include "../TextureManager/TextureManager.h"

DynamicObject::DynamicObject(const Config &config)
    : GameObject(config.gameObject),
      velocity(config.velocity),
      acceleration(config.acceleration),
      maxSpeed(config.maxSpeed),
      friction(config.friction),
      mass(config.mass)
{
}

bool DynamicObject::init()
{
  texture = TextureManager::load(textureID);
  if (texture == nullptr)
    return false;
  return true;
}

void DynamicObject::handleInput() {}
void DynamicObject::update(float deltaTime) {}

void DynamicObject::draw()
{
  TextureManager::draw(texture, position.x, position.y, size.x, size.y);
}

void DynamicObject::shutdown()
{
  texture = nullptr;
}