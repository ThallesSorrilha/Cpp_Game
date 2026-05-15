#include "../include/PhysicalObject.h"
#include "../include/TextureManager.h"

PhysicalObject::PhysicalObject(const Config &config)
    : GameObject(config.gameObject),
      colliderBox(std::make_unique<ColliderBox>(config.colliderBox))
{

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

PhysicalObject::~PhysicalObject() {}

void PhysicalObject::handleInput() {}
void PhysicalObject::update(float deltaTime) { (void)deltaTime; }

void PhysicalObject::draw()
{
    animation.draw(getPosition(), getSize());
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

bool PhysicalObject::isAlive() const
{
    return alive;
}

bool PhysicalObject::hasObjToCreate() const
{
    return hasPendingObjToCreate;
}