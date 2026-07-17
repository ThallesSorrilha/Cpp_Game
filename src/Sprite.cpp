#include "../include/Sprite.h"

#include <utility>

#include "../include/TextureManager.h"
#include "../include/definitions/SpriteCutArray.h"

Sprite::Sprite(SDL_Texture *texture, SpriteCutID spriteCutID)
    : texture(texture), spriteCutID(spriteCutID)
{
    int id = std::to_underlying(this->spriteCutID);
    this->cut = {SpriteCutArray[id][0],
                 SpriteCutArray[id][1],
                 SpriteCutArray[id][2],
                 SpriteCutArray[id][3]};
}

void Sprite::draw(const Vector2D &position, const Vector2D &size) const
{
    TextureManager::draw(texture, position.x, position.y, size.x, size.y, cut);
}