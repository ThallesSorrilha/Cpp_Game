#include "../include/Animation.h"

#include <cstddef>

Animation::Animation(SDL_Texture *texture, const std::vector<SDL_Rect> &cuts)
{
    sprites.reserve(cuts.size());
    for (const SDL_Rect &cut : cuts)
    {
        sprites.emplace_back(texture, cut);
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

//? Talvez tenha que tirar esse método
void Animation::updateFacing(Facing facing)
{
    this->facing = facing;

    const int column = static_cast<int>(facing);
    for (Sprite &sprite : sprites)
    {
        SDL_Rect cut = sprite.getCut();
        cut.x = column * cut.w;
        sprite.setCut(cut);
    }
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
