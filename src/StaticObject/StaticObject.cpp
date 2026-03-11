#include "StaticObject.h"

StaticObject::StaticObject(const Config &config)
    : GameObject(config.gameObject),
      solid(solid),
      interactable(interactable)
{
}

bool StaticObject::init() { return true; }
void StaticObject::handleInput() {}
void StaticObject::update(float deltaTime) {}
void StaticObject::draw() {}
void StaticObject::shutdown() {}