#pragma once
#include "GameObject/GameObject.h"

class StaticObject : public GameObject
{
public:
    bool init() override;
    void handleInput() override;
    void update() override;
    void draw() override;
    void shutdown() override;
};