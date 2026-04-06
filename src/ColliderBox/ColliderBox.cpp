#include "ColliderBox.h"

ColliderBox::ColliderBox(const Config &config)
    : layerCollision(config.layerCollision),
      maskCollision(config.maskCollision),
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

void ColliderBox::setLayerCollision(int newLayerCollision)
{
  layerCollision = newLayerCollision;
}

int ColliderBox::getLayerCollision() const
{
  return layerCollision;
}

void ColliderBox::setMaskCollision(int newMaskCollision)
{
  maskCollision = newMaskCollision;
}

int ColliderBox::getMaskCollision() const
{
  return maskCollision;
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

bool ColliderBox::intersectsAABB(const Vector2D &otherPosition, const Vector2D &otherSize) const
{
  if (!enabled)
  {
    return false;
  }

  const bool overlapX = (position.x < otherPosition.x + otherSize.x) && (position.x + size.x > otherPosition.x);
  const bool overlapY = (position.y < otherPosition.y + otherSize.y) && (position.y + size.y > otherPosition.y);
  return overlapX && overlapY;
}
