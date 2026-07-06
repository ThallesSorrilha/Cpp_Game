#pragma once

#include <SDL2/SDL.h>

#include "utils/Vector2D.h"
#include "enums/SpriteCutID.h"

class Sprite
{
private:
    SDL_Texture *texture = nullptr;
    SpriteCutID spriteCutID;
    SDL_Rect cut = {0, 0, 0, 0};

public:
    Sprite(SDL_Texture *texture, SpriteCutID spriteCutID);
    ~Sprite();

    void draw(const Vector2D &position, const Vector2D &size) const;
};
