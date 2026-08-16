#pragma once

#include <array>
#include <vector>
#include <utility>

#include "../enums/AnimationID.h"

const std::array<std::vector<int>, 3> AnimationSetArray = {

    // 0
    std::vector<int>{
        std::to_underlying(AnimationID::Character_WalkDown),
        std::to_underlying(AnimationID::Character_WalkUp),
        std::to_underlying(AnimationID::Character_WalkLeft),
        std::to_underlying(AnimationID::Character_WalkRight),
        std::to_underlying(AnimationID::Character_AttackDown),
        std::to_underlying(AnimationID::Character_AttackUp),
        std::to_underlying(AnimationID::Character_AttackLeft),
        std::to_underlying(AnimationID::Character_AttackRight),
        std::to_underlying(AnimationID::Character_IdleDown),
        std::to_underlying(AnimationID::Character_IdleUp),
        std::to_underlying(AnimationID::Character_IdleLeft),
        std::to_underlying(AnimationID::Character_IdleRight),
        std::to_underlying(AnimationID::Character_Dead)
    },

    // 1
    std::vector<int>{
        std::to_underlying(AnimationID::Attack_Down),
        std::to_underlying(AnimationID::Attack_Up),
        std::to_underlying(AnimationID::Attack_Left),
        std::to_underlying(AnimationID::Attack_Right)},

    // 2
    std::vector<int>{
        std::to_underlying(AnimationID::Coin)}};