#pragma once
#include <string>
#include <map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class TextureManager
{
public:
    static bool init(SDL_Renderer* renderer);
    static SDL_Texture *load(const std::string& fileName);
    static void draw(SDL_Texture* texture, float x, float y, float w, float h);
    static void shutdown();

private:
    static inline SDL_Renderer *renderer = nullptr;
    static inline std::map<std::string, SDL_Texture*> textureMap;
};