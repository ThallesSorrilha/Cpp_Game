#include "../include/StaticObject.h"

StaticObject::StaticObject(const Config &config)
    : PhysicalObject(config.physicalObject)
{
}

void StaticObject::handleInput() {}

void StaticObject::update(float deltaTime)
{
    (void)deltaTime;
    updateCurrentAnimation();
}

void StaticObject::draw()
{
    PhysicalObject::draw();
}

void StaticObject::onCollision(const PhysicalObject &otherObject) { (void)otherObject; }
