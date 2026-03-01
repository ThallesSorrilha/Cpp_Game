#pragma once
#include "DynamicObject/DynamicObject.h"

class Character : public DynamicObject
{
public:
    bool init() override;
    void handleInput() override;
    void update() override;
    void draw() override;
    void shutdown() override;
};