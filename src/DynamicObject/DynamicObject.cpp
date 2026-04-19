#include "DynamicObject.h"

#include <stdexcept>
#include <string>

#include "../TileMap/TileMap.h"

DynamicObject::DynamicObject(const Config &config)
    : PhysicalObject(config.physicalObject),
      velocity(config.velocity),
      acceleration(config.acceleration),
      maxSpeed(config.maxSpeed),
      friction(config.friction),
      mass(config.mass)
{
}

void DynamicObject::handleInput() {}
void DynamicObject::update(float deltaTime) { (void)deltaTime; }
void DynamicObject::draw() {}

void DynamicObject::setCollisionMap(const TileMap *newCollisionMap)
{
  collisionMap = newCollisionMap;
}

const TileMap *DynamicObject::getCollisionMap() const
{
  return collisionMap;
}
