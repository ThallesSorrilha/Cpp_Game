#include "../include/PhysicalObject.h"
#include "../include/TextureManager.h"

#include <string>
#include <stdexcept>
#include <utility>

PhysicalObject::PhysicalObject(const Config &config)
    : GameObject(config.gameObject),
      colliderBox(std::make_unique<ColliderBox>(config.colliderBox))
{
    texture = TextureManager::load(spriteID);
    if (texture == nullptr)
    {
        throw std::runtime_error("PhysicalObject ctor error: failed to load texture id " + std::to_string(static_cast<int>(spriteID)));
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

PhysicalObject::~PhysicalObject() = default;

void PhysicalObject::handleInput() {}

void PhysicalObject::update(float deltaTime) { (void)deltaTime; }

void PhysicalObject::draw()
{
    Animation *currentAnimation = getCurrentAnimation();
    if (currentAnimation != nullptr)
    {
        currentAnimation->draw(getPosition(), getSize());
    }
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

void PhysicalObject::initializeAnimations(const std::vector<AnimationID> &animationIDs)
{
    animationsIDs = animationIDs;
    animations.clear();
    animationMap.clear();

    animations.reserve(animationsIDs.size());
    for (std::size_t animationIndex = 0; animationIndex < animationsIDs.size(); ++animationIndex)
    {
        const AnimationID animationID = animationsIDs[animationIndex];
        animations.emplace_back(texture, animationID);
        animationMap[std::to_underlying(animationID)] = animationIndex;
    }

    currentAnimation = 0;
}

void PhysicalObject::swapAnimation(AnimationID animationID)
{
    const auto animationIndex = animationMap.find(std::to_underlying(animationID));
    if (animationIndex != animationMap.end())
    {
        currentAnimation = animationIndex->second;
    }
}

void PhysicalObject::updateCurrentAnimation()
{
    Animation *currentAnimationObject = getCurrentAnimation();
    if (currentAnimationObject != nullptr)
    {
        currentAnimationObject->update();
    }
}

void PhysicalObject::setCurrentAnimation(std::size_t index)
{
    if (index < animations.size())
    {
        currentAnimation = index;
    }
}

Animation *PhysicalObject::getCurrentAnimation()
{
    if (animations.empty() || currentAnimation >= animations.size())
    {
        return nullptr;
    }

    return &animations[currentAnimation];
}

SDL_Texture *PhysicalObject::getTexture() const
{
    return texture;
}

bool PhysicalObject::isAlive() const
{
    return alive;
}

bool PhysicalObject::hasObjToCreate() const
{
    return hasPendingObjToCreate;
}