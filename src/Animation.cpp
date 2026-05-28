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
    if (row > 3)
    {
        row = 0;
        return;
    }
    row += 1;
}

void Animation::updateFacing(Facing facing)
{
    column = std::to_underlying(facing);
}

void Animation::draw(const Vector2D &position, const Vector2D &size)
{
    TextureManager::drawTile(texture, position.x, position.y, size.x, size.y, row, column);
}
