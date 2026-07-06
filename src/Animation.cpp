#include "../include/Animation.h"

#include <cstddef>

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

Animation::~Animation()
{
}

void Animation::nextFrame()
{
    if (sprites.empty())
    {
        return;
    }

    if (currentSprite + 1 >= sprites.size())
    {
        if (repeat)
        {
            currentSprite = 0;
        }
        return;
    }

    currentSprite += 1;
}

void Animation::draw(const Vector2D &position, const Vector2D &size) const
{
    if (sprites.empty())
    {
        return;
    }

    //? Fica somando infinitamente?
    const std::size_t index = currentSprite < sprites.size() ? currentSprite : 0;
    sprites[index].draw(position, size);
}
