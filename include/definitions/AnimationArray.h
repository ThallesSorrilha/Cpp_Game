#pragma once

#include <array>
#include <vector>
#include <utility>

#include "../enums/SpriteCutID.h"

const std::array<std::vector<int>, 12> AnimationArray = {
    std::vector<int>{std::to_underlying(SpriteCutID::Character_WalkDown1), std::to_underlying(SpriteCutID::Character_WalkDown2), std::to_underlying(SpriteCutID::Character_WalkDown3), std::to_underlying(SpriteCutID::Character_WalkDown4)},     // 0
    std::vector<int>{std::to_underlying(SpriteCutID::Character_WalkUp1), std::to_underlying(SpriteCutID::Character_WalkUp2), std::to_underlying(SpriteCutID::Character_WalkUp3), std::to_underlying(SpriteCutID::Character_WalkUp4)},             // 1
    std::vector<int>{std::to_underlying(SpriteCutID::Character_WalkLeft1), std::to_underlying(SpriteCutID::Character_WalkLeft2), std::to_underlying(SpriteCutID::Character_WalkLeft3), std::to_underlying(SpriteCutID::Character_WalkLeft4)},     // 2
    std::vector<int>{std::to_underlying(SpriteCutID::Character_WalkRight1), std::to_underlying(SpriteCutID::Character_WalkRight2), std::to_underlying(SpriteCutID::Character_WalkRight3), std::to_underlying(SpriteCutID::Character_WalkRight4)}, // 3
    std::vector<int>{std::to_underlying(SpriteCutID::Character_AttackDown)},                                                                                                                                                                      // 4
    std::vector<int>{std::to_underlying(SpriteCutID::Character_AttackUp)},                                                                                                                                                                        // 5
    std::vector<int>{std::to_underlying(SpriteCutID::Character_AttackLeft)},                                                                                                                                                                      // 6
    std::vector<int>{std::to_underlying(SpriteCutID::Character_AttackRight)},                                                                                                                                                                     // 7
    std::vector<int>{std::to_underlying(SpriteCutID::Attack_Down1), std::to_underlying(SpriteCutID::Attack_Down2), std::to_underlying(SpriteCutID::Attack_Down3), std::to_underlying(SpriteCutID::Attack_Down4)},                                 // 8
    std::vector<int>{std::to_underlying(SpriteCutID::Attack_Up1), std::to_underlying(SpriteCutID::Attack_Up2), std::to_underlying(SpriteCutID::Attack_Up3), std::to_underlying(SpriteCutID::Attack_Up4)},                                         // 9
    std::vector<int>{std::to_underlying(SpriteCutID::Attack_Left1), std::to_underlying(SpriteCutID::Attack_Left2), std::to_underlying(SpriteCutID::Attack_Left3), std::to_underlying(SpriteCutID::Attack_Left4)},                                 // 10
    std::vector<int>{std::to_underlying(SpriteCutID::Attack_Right1), std::to_underlying(SpriteCutID::Attack_Right2), std::to_underlying(SpriteCutID::Attack_Right3), std::to_underlying(SpriteCutID::Attack_Right4)},                             // 11
};