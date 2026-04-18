#pragma once

#include <SDL2/SDL.h>

#include "../utils/Vector2D.h"
#include "../enums/SpriteID.h"

class GameObject
{
public:
    struct Config
    {
        Vector2D position = {0.0f, 0.0f};
        Vector2D size = {1.0f, 1.0f};
        SpriteID spriteID = SpriteID::Player;
        SDL_Texture *texture = nullptr;
    };
    explicit GameObject(const Config &config);
    virtual ~GameObject() = default;

    virtual void handleInput() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void draw() = 0;

    const Vector2D &getPosition() const { return position; }
    const Vector2D &getSize() const { return size; }

protected:
    Vector2D position;
    Vector2D size;
    SpriteID spriteID;
    SDL_Texture *texture;
};