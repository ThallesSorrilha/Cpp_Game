#include <string>
#include <iostream>
#include "TextureManager.h"
#include "../utils/Definitions.h"

bool TextureManager::init(SDL_Renderer *r)
{
    renderer = r;
    if (renderer == nullptr)
    {
        std::cout << "TextureManager renderer is null" << std::endl;
        return false;
    }
    return true;
}

SDL_Texture *TextureManager::load(std::string filename)
{
    if (renderer == nullptr)
    {
        std::cout << "TextureManager renderer is null" << std::endl;
        return nullptr;
    }

    SDL_Surface *surface = IMG_Load((SPRITES_PATH + filename).c_str());
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

void TextureManager::draw(SDL_Texture *texture, int x, int y, int w, int h)
{
    if (renderer == nullptr)
    {
        std::cout << "TextureManager renderer is null" << std::endl;
        return;
    }

    SDL_Rect srcRect{0, 0, w, h};
    SDL_Rect destRect{x, y, w, h};

    SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
}

void TextureManager::shutdown()
{
    renderer = nullptr;
}