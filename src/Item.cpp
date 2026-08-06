#include "../include/Item.h"

#include "../include/enums/AnimationID.h"

Item::Item(const Config &config)
    : StaticObject(config.staticObject)
{
    switch (config.itemType)
    {
    case ItemType::Coin:
        initializeAnimations({AnimationID::Coin});
        break;
    }
}

void Item::handleInput() {}

void Item::update(float deltaTime)
{
    StaticObject::update(deltaTime);
}

void Item::draw()
{
    StaticObject::draw();
}

void Item::onCollision(const PhysicalObject &otherObject) { (void)otherObject; }
