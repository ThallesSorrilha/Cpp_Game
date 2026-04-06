#pragma once
#include <string>
#include <map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../utils/Vector2D.h"
#include "../enums/TextureID.h"

class TextureManager
{
public:
    static bool init(SDL_Renderer *renderer);
    static SDL_Texture *load(const TextureID &id);
    static void setCameraPosition(const Vector2D &newCameraPosition);
    static void clearCamera();
    static void draw(SDL_Texture *texture, float x, float y, float w, float h);
    static void drawTile(SDL_Texture *texture, float x, float y, float w, float h, int row, int col);
    static void shutdown();

private:
    static inline SDL_Renderer *renderer = nullptr;
    static inline std::map<TextureID, SDL_Texture *> textureMap;
    static inline Vector2D cameraPosition = {0.0f, 0.0f};
};