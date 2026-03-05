#pragma once
#include "Character/Character.h"

class Player : public Character
{
public:
    bool init() override;
    void handleInput() override;
    void update() override;
    void draw() override;
    void shutdown() override;

private:
    Vector2D inputDirection;
    int coins = 0;
};