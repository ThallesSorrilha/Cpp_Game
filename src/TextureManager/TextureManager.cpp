#include <string>
#include <iostream>

#include "TextureManager.h"
#include "../GameManager/GameManager.h"

SDL_Texture *TextureManager::loadTexture(std::string filename)
{
    SDL_Surface *surface = IMG_Load(filename.c_str());
    if (surface == nullptr)
        std::cout << "Surface error" << std::endl;

    SDL_Texture *texture = SDL_CreateTextureFromSurface(GameManager::getRenderer(), surface);
    if (texture == nullptr)
        std::cout << "Texture error" << std::endl;

    SDL_FreeSurface(surface);
    return texture;
}

void TextureManager::draw(SDL_Texture *texture, const SDL_Rect &srcRect, const SDL_Rect &destRect)
{
    SDL_RenderCopy(GameManager::getRenderer(), texture, &srcRect, &destRect);
}