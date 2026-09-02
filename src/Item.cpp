#include "../include/Item.h"

#include "../include/enums/AnimationID.h"
#include "../include/enums/LayerID.h"

Item::Item(const Config &config)
    : StaticObject(config.staticObject)
{
    colliderBox->setCollisionLayer(LayerUtils::toMask(LayerID::Item));
    colliderBox->setCollisionMask(LayerUtils::toMask(LayerID::Player));

    switch (config.itemType)
    {
    case ItemType::Coin:
        initializeAnimations(AnimationSet::Item);
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

void Item::collect() const
{
    markForRemoval();
}
