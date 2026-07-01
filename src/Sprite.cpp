#include "../include/Sprite.h"

#include "../include/TextureManager.h"

Sprite::Sprite(SDL_Texture *texture, SDL_Rect cut)
    : texture(texture), cut(cut)
{
}

Sprite::~Sprite()
{
}

void Sprite::setCut(const SDL_Rect &newCut)
{
    cut = newCut;
}

const SDL_Rect &Sprite::getCut() const
{
    return cut;
}

SDL_Texture *Sprite::getTexture() const
{
    return texture;
}

//? por que precisa de position, size e cut?
void Sprite::draw(const Vector2D &position, const Vector2D &size) const
{
    TextureManager::draw(texture, position.x, position.y, size.x, size.y, cut);
}