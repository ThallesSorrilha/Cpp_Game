#include "../include/Item.h"

Item::Item(const Config &config)
    : StaticObject(config.staticObject)
{
}

void Item::handleInput() {}
void Item::update(float deltaTime) { (void)deltaTime; }
void Item::draw() {}
void Item::onCollision(const PhysicalObject &otherObject) { (void)otherObject; }
