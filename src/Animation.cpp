#include "../include/Animation.h"

#include <cstddef>
#include <utility>

#include "../include/definitions/AnimationArray.h"

Animation::Animation(SDL_Texture *texture, AnimationID animationID)
    : animationID(animationID)
{
    int id = std::to_underlying(animationID);
    const auto &spriteIDs = AnimationArray[id];
    sprites.reserve(spriteIDs.size());
    for (int spriteID : spriteIDs)
    {
        sprites.emplace_back(texture, static_cast<SpriteCutID>(spriteID));
    }
}

void Animation::update()
{
    if (sprites.empty())
    {
        return;
    }

    if (spriteTimer.isEnd())
    {
        this->spriteTimer.setTimer(this->frameTime);
        currentSprite += 1;
    }

    if (currentSprite >= sprites.size())
    {
        if (repeat)
        {
            currentSprite = 0;
        }
        return;
    }
}

void Animation::draw(const Vector2D &position, const Vector2D &size) const
{
    if (sprites.empty())
    {
        return;
    }

    const std::size_t index = currentSprite < sprites.size() ? currentSprite : 0;
    sprites[index].draw(position, size);
}
