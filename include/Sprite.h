#pragma once

#include <SDL2/SDL.h>

#include "utils/Vector2D.h"

class Sprite
{
private:
    SDL_Texture *texture = nullptr;
    SDL_Rect cut = {0, 0, 16, 16};

public:
    Sprite() = default;
    Sprite(SDL_Texture *texture, int tile, int w, int h);
    ~Sprite();

    void setCut(const SDL_Rect &newCut);
    const SDL_Rect &getCut() const;
    SDL_Texture *getTexture() const;
    void draw(const Vector2D &position, const Vector2D &size) const;
};
