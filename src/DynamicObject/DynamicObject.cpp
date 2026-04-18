#include "DynamicObject.h"

#include <stdexcept>
#include <string>

#include "../TextureManager/TextureManager.h"
#include "../TileMap/TileMap.h"

DynamicObject::DynamicObject(const Config &config)
    : GameObject(config.gameObject),
      velocity(config.velocity),
      acceleration(config.acceleration),
      maxSpeed(config.maxSpeed),
      friction(config.friction),
      mass(config.mass),
      colliderBox(std::make_unique<ColliderBox>(config.colliderBox))
{
  texture = TextureManager::load(spriteID);
  if (texture == nullptr)
  {
    throw std::runtime_error("DynamicObject ctor error: failed to load texture id " + std::to_string(static_cast<int>(spriteID)));
  }

  if (colliderBox)
  {
    const Vector2D &colliderSize = colliderBox->getSize();
    if (colliderSize.x <= 0.0f || colliderSize.y <= 0.0f)
    {
      colliderBox->setSize(size);
    }
    colliderBox->syncToObjectPosition(position);
  }
}

DynamicObject::~DynamicObject()
{
  texture = nullptr;
}

void DynamicObject::handleInput() {}
void DynamicObject::update(float deltaTime)
{
  (void)deltaTime;
  syncColliderToPosition();
}

void DynamicObject::draw()
{
  TextureManager::draw(texture, position.x, position.y, size.x, size.y);
}

void DynamicObject::setCollisionMap(const TileMap *newCollisionMap)
{
  collisionMap = newCollisionMap;
}

const TileMap *DynamicObject::getCollisionMap() const
{
  return collisionMap;
}

ColliderBox *DynamicObject::getColliderBox() const
{
  return colliderBox.get();
}

void DynamicObject::syncColliderToPosition()
{
  if (colliderBox)
  {
    colliderBox->syncToObjectPosition(position);
  }
}
