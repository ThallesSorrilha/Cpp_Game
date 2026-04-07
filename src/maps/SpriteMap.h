#pragma once

#include <map>
#include <string>

#include "../enums/TextureID.h"

const std::map<TextureID, std::string> SpriteMap = {
    {TextureID::Enemy, "assets/sprites/enemy.png"},
    {TextureID::Player, "assets/sprites/player.png"},
    {TextureID::World01, "assets/sprites/world01.png"},
};