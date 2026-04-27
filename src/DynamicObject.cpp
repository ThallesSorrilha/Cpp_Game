#include "../include/DynamicObject.h"

#include <stdexcept>
#include <string>

#include "../include/TileMap.h"
#include "../include/ColliderManager.h"

DynamicObject::DynamicObject(const Config &config)
    : PhysicalObject(config.physicalObject),
      force(config.force),
      velocity(config.velocity),
      acceleration(config.acceleration),
      maxSpeed(config.maxSpeed),
      friction(config.friction),
      mass(config.mass)
{
}

void DynamicObject::handleInput() {}

void DynamicObject::update(float deltaTime)
{
  velocity = force * maxSpeed;
  position += velocity * deltaTime;
  syncColliderToPosition();
}

void DynamicObject::draw() {}

void DynamicObject::setCollisionMap(const TileMap *newCollisionMap)
{
  collisionMap = newCollisionMap;
}

const TileMap *DynamicObject::getCollisionMap() const
{
  return collisionMap;
}
