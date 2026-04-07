#include "StaticObject.h"

StaticObject::StaticObject(const Config &config)
    : GameObject(config.gameObject),
      solid(config.solid),
      interactable(config.interactable)
{
}

void StaticObject::handleInput() {}

void StaticObject::update(float deltaTime)
{
  (void)deltaTime;
}

void StaticObject::draw() {}
