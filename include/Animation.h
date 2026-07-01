#include <SDL2/SDL.h>
#include <vector>

#include "Sprite.h"
#include "utils/Vector2D.h"
#include "enums/Facing.h"

/*
Animação é um vetor de classe de sprites
Qual dos sprites está
quanto tempo para cada sprite
cronômetro de animação
*/

class Animation
{
private:
    std::vector<Sprite> sprites;
    std::size_t currentSprite = 0;
    float frameTime = 0.2f;
    bool repeat = true;

public:
    Animation() = default;
    Animation(SDL_Texture *texture, const std::vector<SDL_Rect> &cuts);
    ~Animation();

    void nextFrame();
    void updateFacing(Facing facing);
    void draw(const Vector2D &position, const Vector2D &size) const;
};
