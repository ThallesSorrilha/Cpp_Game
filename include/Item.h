#pragma once

#include "StaticObject.h"

#include "enums/ItemType.h"

class Item : public StaticObject
{
public:
    struct Config
    {
        StaticObject::Config staticObject;
        ItemType itemType = ItemType::Coin;
    };
    explicit Item(const Config &config);

    void handleInput() override;
    void update(float deltaTime) override;
    void draw() override;
    void onCollision(const PhysicalObject &otherObject) override;
};