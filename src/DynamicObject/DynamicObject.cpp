#include "DynamicObject.h"

DynamicObject::DynamicObject(const Config &config)
    : GameObject(config.gameObject),
      velocity(config.velocity),
      acceleration(config.acceleration),
      maxSpeed(config.maxSpeed),
      friction(config.friction),
      mass(config.mass)
{
}

bool DynamicObject::init() { return true; }
void DynamicObject::handleInput() {}
void DynamicObject::update() {}
void DynamicObject::draw() {}
void DynamicObject::shutdown() {}