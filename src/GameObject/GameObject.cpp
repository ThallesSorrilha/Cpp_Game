#include "GameObject.h"

GameObject::GameObject(const Config &config)
    : position(config.position),
      size(config.size),
      textureFilePath(config.textureFilePath),
      texture(config.texture)
{
}