#include "../include/DynamicObject.h"

#include <stdexcept>
#include <string>

#include "../include/TileMap.h"
#include "../include/ColliderManager.h"
#include "../include/definitions/Definitions.h"

DynamicObject::DynamicObject(const Config &config)
    : PhysicalObject(config.physicalObject),
      force(config.force),
      velocity(config.velocity),
      acceleration(config.acceleration),
      friction(config.friction),
      mass(config.mass)
{
}

void DynamicObject::handleInput() {}

void DynamicObject::update(float deltaTime)
{
  float frictionApplied = mass * GRAVITY_FORCE * friction;
  const Vector2D frictionForce = velocity * frictionApplied;
  acceleration = (force - frictionForce) * (1.0f / mass);
  velocity += acceleration * deltaTime;
  position += velocity * deltaTime;

  syncColliderToPosition();

  force = {0.0f, 0.0f};
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
