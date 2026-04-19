#include "StaticObject.h"

StaticObject::StaticObject(const Config &config)
    : PhysicalObject(config.physicalObject)
{
}

void StaticObject::handleInput() {}
void StaticObject::update(float deltaTime) { (void)deltaTime; }
void StaticObject::draw() {}
