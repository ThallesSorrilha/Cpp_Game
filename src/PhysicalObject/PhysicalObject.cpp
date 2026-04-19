#include "PhysicalObject.h"

#include "../TextureManager/TextureManager.h"

PhysicalObject::PhysicalObject(const Config &config)
    : GameObject(config.gameObject),
      colliderBox(std::make_unique<ColliderBox>(config.colliderBox))
{
    texture = TextureManager::load(spriteID);
    if (texture == nullptr)
    {
        throw std::runtime_error("DynamicObject ctor error: failed to load texture id " + std::to_string(static_cast<int>(spriteID)));
    }

    if (colliderBox)
    {
        const Vector2D &colliderSize = colliderBox->getSize();
        if (colliderSize.x <= 0.0f || colliderSize.y <= 0.0f)
        {
            colliderBox->setSize(size);
        }
        colliderBox->syncToObjectPosition(position);
    }
}

PhysicalObject::~PhysicalObject()
{
  texture = nullptr;
}

void PhysicalObject::handleInput() {}
void PhysicalObject::update(float deltaTime) { (void)deltaTime; }

void PhysicalObject::draw()
{
    TextureManager::draw(texture, position.x, position.y, size.x, size.y);
}

void PhysicalObject::syncColliderToPosition()
{
    if (colliderBox)
    {
        colliderBox->syncToObjectPosition(position);
    }
}

ColliderBox *PhysicalObject::getColliderBox() const
{
    return colliderBox.get();
}