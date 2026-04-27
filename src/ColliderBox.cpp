#include "../include/ColliderBox.h"

ColliderBox::ColliderBox(const Config &config)
    : collisionLayer(config.collisionLayer),
      collisionMask(config.collisionMask),
      enabled(config.enabled),
      offset(config.offset),
      position(config.offset),
      size(config.size)
{
}

void ColliderBox::setEnabled(bool newEnabled)
{
  enabled = newEnabled;
}

bool ColliderBox::isEnabled() const
{
  return enabled;
}

void ColliderBox::setCollisionLayer(std::uint32_t newCollisionLayer)
{
  collisionLayer = newCollisionLayer;
}

std::uint32_t ColliderBox::getCollisionLayer() const
{
  return collisionLayer;
}

void ColliderBox::setCollisionMask(std::uint32_t newCollisionMask)
{
  collisionMask = newCollisionMask;
}

std::uint32_t ColliderBox::getCollisionMask() const
{
  return collisionMask;
}

void ColliderBox::setOffset(const Vector2D &newOffset)
{
  offset = newOffset;
}

const Vector2D &ColliderBox::getOffset() const
{
  return offset;
}

void ColliderBox::setSize(const Vector2D &newSize)
{
  size = newSize;
}

const Vector2D &ColliderBox::getSize() const
{
  return size;
}

void ColliderBox::syncToObjectPosition(const Vector2D &objectPosition)
{
  position = objectPosition + offset;
}

const Vector2D &ColliderBox::getPosition() const
{
  return position;
}
