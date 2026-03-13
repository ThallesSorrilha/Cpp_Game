#include <string>
#include <iostream>

#include "TextureManager.h"
#include "../GameManager/GameManager.h"

bool TextureManager::init()
{
    renderer = GameManager::getRenderer();
    if (renderer == nullptr)
    {
        std::cout << "TextureManager renderer is null" << std::endl;
        return false;
    }
    return true;
}

SDL_Texture *TextureManager::loadTexture(std::string filename)
{
    if (renderer == nullptr)
    {
        std::cout << "TextureManager renderer is null" << std::endl;
        return nullptr;
    }

    SDL_Surface *surface = IMG_Load(filename.c_str());
    if (surface == nullptr)
    {
        std::cout << "IMG_Load error: " << IMG_GetError() << std::endl;
        return nullptr;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == nullptr)
    {
        std::cout << "SDL_CreateTextureFromSurface error: " << SDL_GetError() << std::endl;
    }
    
    SDL_FreeSurface(surface);
    return texture;
}

void TextureManager::draw(SDL_Texture *texture, const SDL_Rect &srcRect, const SDL_Rect &destRect)
{
    if (renderer == nullptr)
    {
        std::cout << "TextureManager renderer is null" << std::endl;
        return;
    }

    SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
}

void TextureManager::shutdown()
{
    renderer = nullptr;
}