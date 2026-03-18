#pragma once
#include <string>
#include <map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class TextureManager
{
public:
    static bool init(SDL_Renderer* renderer);
    static SDL_Texture *load(const std::string fileName);
    static SDL_Texture* get(const std::string& fileName);
    static void unload(const std::string& id);
    static void draw(SDL_Texture* texture, int x, int y, int w, int h);
    static void shutdown();

private:
    static inline SDL_Renderer *renderer = nullptr;
};