#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "TextureManager.h"
#include "utils/Vector2D.h"
#include "enums/SpriteID.h"
#include "enums/Facing.h"

class Animation
{
private:
    SDL_Texture *texture = nullptr;
    int row = 0;
    int column = 0;
    float frame = 0.2;
    bool repeat = true;

public:
    Animation(SpriteID spriteID);
    ~Animation();

    void nextFrame();
    void updateFacing(Facing facing);
    void draw(const Vector2D &position, const Vector2D &size);
};
