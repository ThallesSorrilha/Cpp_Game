#pragma once

#include "../PhysicalObject/PhysicalObject.h"

class StaticObject : public PhysicalObject
{
public:
    struct Config
    {
        PhysicalObject::Config physicalObject;
    };
    explicit StaticObject(const Config &config);

    void handleInput() override;
    void update(float deltaTime) override;
    void draw() override;
    void onCollision(const PhysicalObject &otherObject) override;
};