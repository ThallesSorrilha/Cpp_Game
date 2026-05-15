#include "../include/Animation.h"

Animation::Animation(SpriteID spriteID)
{
    texture = TextureManager::load(spriteID);
    if (texture == nullptr)
    {
        throw std::runtime_error("DynamicObject ctor error: failed to load texture id " + std::to_string(static_cast<int>(spriteID)));
    }
}

Animation::~Animation()
{
    texture = nullptr;
}

void Animation::nextFrame()
{
    if (column > 3)
    {
        column = 0;
        return;
    }
    column += 1;
}

void Animation::draw(const Vector2D &position, const Vector2D &size)
{
    TextureManager::drawTile(texture, position.x, position.y, size.x, size.y, row, column);
}
