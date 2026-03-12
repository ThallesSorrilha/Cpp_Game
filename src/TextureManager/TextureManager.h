#pragma once
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class TextureManager
{
public:
    static SDL_Texture *loadTexture(std::string filename);
    static void draw(SDL_Texture *texture, const SDL_Rect &srcRect, const SDL_Rect &destRect);
};