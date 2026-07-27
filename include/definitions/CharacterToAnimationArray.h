#pragma once

#include <array>

#include "../enums/AnimationID.h"

constexpr std::array<AnimationID, 5> CharacterToAnimationArray = {
    AnimationID::Character_IdleDown,   // Idle
    AnimationID::Character_WalkDown,   // Walking
    AnimationID::Character_AttackDown, // Attacking
    AnimationID::Character_IdleDown,   // Suffering_Damage
    AnimationID::Character_IdleDown    // Dead
};