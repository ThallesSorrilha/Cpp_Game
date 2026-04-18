#include "GameObject.h"

GameObject::GameObject(const Config &config)
    : position(config.position),
      size(config.size),
      spriteID(config.spriteID),
      texture(config.texture)
{
}