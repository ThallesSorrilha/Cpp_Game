#include "DynamicObject.h"
#include <stdexcept>
#include <string>
#include "../TextureManager/TextureManager.h"

DynamicObject::DynamicObject(const Config &config)
    : GameObject(config.gameObject),
      velocity(config.velocity),
      acceleration(config.acceleration),
      maxSpeed(config.maxSpeed),
      friction(config.friction),
      mass(config.mass)
{
  texture = TextureManager::load(textureID);
  if (texture == nullptr)
  {
    throw std::runtime_error("DynamicObject ctor error: failed to load texture id " + std::to_string(static_cast<int>(textureID)));
  }
}

DynamicObject::~DynamicObject()
{
  texture = nullptr;
}

void DynamicObject::handleInput() {}
void DynamicObject::update(float deltaTime) {}

void DynamicObject::draw()
{
  TextureManager::draw(texture, position.x, position.y, size.x, size.y);
}
