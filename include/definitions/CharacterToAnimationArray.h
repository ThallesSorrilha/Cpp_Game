#pragma once

#include <array>

#include "../enums/AnimationID.h"

constexpr std::array<AnimationID, 3> CharacterToAnimationArray = {
    AnimationID::Character_IdleDown,  // 0
    AnimationID::Character_WalkDown,  // 1
    AnimationID::Character_AttackDown // 2
};