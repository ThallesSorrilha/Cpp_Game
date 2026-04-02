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

SDL_Texture *TextureManager::load(const std::string &filename)
{
    if (renderer == nullptr)
    {
        std::cout << "TextureManager renderer is null" << std::endl;
        return nullptr;
    }

    if (textureMap.find(filename) != textureMap.end())
    {
        return textureMap[filename];
    }

    SDL_Surface *surface = IMG_Load((filename).c_str());
    if (surface == nullptr)
    {
        std::cout << "IMG_Load error: " << IMG_GetError() << std::endl;
        return nullptr;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == nullptr)
    {
        std::cout << "SDL_CreateTextureFromSurface error: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(surface);
        return nullptr;
    }

    textureMap[filename] = texture;

    SDL_FreeSurface(surface);
    return texture;
}

void TextureManager::setCameraPosition(const Vector2D &newCameraPosition)
{
    cameraPosition = newCameraPosition;
}

void TextureManager::clearCamera()
{
    cameraPosition = {0.0f, 0.0f};
}

void TextureManager::draw(SDL_Texture *texture, float x, float y, float w, float h)
{
    if (renderer == nullptr)
    {
        std::cout << "TextureManager renderer is null" << std::endl;
        return;
    }

    SDL_Rect srcRect{
        0,
        0,
        static_cast<int>(w * PIXELS_PER_TILE),
        static_cast<int>(h * PIXELS_PER_TILE)};
    SDL_Rect destRect{
        static_cast<int>((x - cameraPosition.x) * PIXELS_PER_BLOCK),
        static_cast<int>((y - cameraPosition.y) * PIXELS_PER_BLOCK),
        static_cast<int>(w * PIXELS_PER_BLOCK),
        static_cast<int>(h * PIXELS_PER_BLOCK)};
    SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
}

void TextureManager::drawTile(SDL_Texture *texture, float x, float y, float w, float h, int row, int col)
{
    if (renderer == nullptr)
    {
        std::cout << "TextureManager renderer is null" << std::endl;
        return;
    }

    SDL_Rect srcRect{
        col * PIXELS_PER_TILE,
        row * PIXELS_PER_TILE,
        static_cast<int>(w * PIXELS_PER_TILE),
        static_cast<int>(h * PIXELS_PER_TILE)};
    SDL_Rect destRect{
        static_cast<int>((x - cameraPosition.x) * PIXELS_PER_BLOCK),
        static_cast<int>((y - cameraPosition.y) * PIXELS_PER_BLOCK),
        static_cast<int>(w * PIXELS_PER_BLOCK),
        static_cast<int>(h * PIXELS_PER_BLOCK)};
    SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
}

void TextureManager::shutdown()
{
    for (auto &pair : textureMap)
    {
        SDL_DestroyTexture(pair.second);
    }
    textureMap.clear();
    clearCamera();
    renderer = nullptr;
}