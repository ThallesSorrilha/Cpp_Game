#include "GameObject.h"

GameObject::GameObject(const Config &config)
    : position(config.position),
      size(config.size),
      textureID(config.textureID),
      texture(config.texture)
{
}