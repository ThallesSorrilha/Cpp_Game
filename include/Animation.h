#pragma once

#include <SDL2/SDL.h>
#include <vector>

#include "Sprite.h"
#include "utils/Vector2D.h"
#include "enums/Facing.h"
#include "enums/AnimationID.h"

class Animation
{
private:
    std::vector<Sprite> sprites;
    std::size_t currentSprite = 0;
    AnimationID animationID;
    float frameTime = 0.2f;
    bool repeat = false;

public:
    Animation(SDL_Texture *texture, AnimationID animationID);
    ~Animation() = default;

    void nextFrame();
    void draw(const Vector2D &position, const Vector2D &size) const;
};
