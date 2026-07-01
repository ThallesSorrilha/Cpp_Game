#include "../include/PhysicalObject.h"
#include "../include/TextureManager.h"

#include <stdexcept>
#include <string>
#include <vector>

#include "../include/definitions/Definitions.h"

PhysicalObject::PhysicalObject(const Config &config)
    : GameObject(config.gameObject),
      colliderBox(std::make_unique<ColliderBox>(config.colliderBox))
{
    texture = TextureManager::load(spriteID);
    if (texture == nullptr)
    {
        throw std::runtime_error("PhysicalObject ctor error: failed to load texture id " + std::to_string(static_cast<int>(spriteID)));
    }

    int textureWidth = 0;
    int textureHeight = 0;
    if (SDL_QueryTexture(texture, nullptr, nullptr, &textureWidth, &textureHeight) != 0)
    {
        throw std::runtime_error("PhysicalObject ctor error: failed to query texture size for id " + std::to_string(static_cast<int>(spriteID)));
    }

    constexpr int kSpriteGridSize = 4;
    const int frameWidth = textureWidth / kSpriteGridSize;
    const int frameHeight = textureHeight / kSpriteGridSize;
    if (frameWidth <= 0 || frameHeight <= 0)
    {
        throw std::runtime_error("PhysicalObject ctor error: invalid sprite frame size for id " + std::to_string(static_cast<int>(spriteID)));
    }

    std::vector<SDL_Rect> cuts;
    cuts.reserve(kSpriteGridSize);
    for (int frameIndex = 0; frameIndex < kSpriteGridSize; ++frameIndex)
    {
        cuts.push_back(SDL_Rect{0, frameIndex * frameHeight, frameWidth, frameHeight});
    }
    animations.emplace_back(texture, cuts);

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