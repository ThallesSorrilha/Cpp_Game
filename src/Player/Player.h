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
};