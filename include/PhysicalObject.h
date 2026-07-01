#pragma once

#include <cstddef>
#include <memory>
#include <vector>

#include <SDL2/SDL.h>

#include "GameObject.h"
#include "ColliderBox.h"
#include "Animation.h"

class PhysicalObject : public GameObject
{
public:
    struct Config
    {
        GameObject::Config gameObject;
        ColliderBox::Config colliderBox;
    };
    explicit PhysicalObject(const Config &config);
    virtual ~PhysicalObject() override;

    void handleInput() override;
    void update(float deltaTime) override;
    void draw() override;
    ColliderBox *getColliderBox() const;
    void syncColliderToPosition();
    void setCurrentAnimation(std::size_t index);
    Animation *getCurrentAnimation();
    SDL_Texture *getTexture() const;
    virtual void onCollision(const PhysicalObject &otherObject) = 0;
    bool isAlive() const;
    bool hasObjToCreate() const;

protected:
    SDL_Texture *texture = nullptr;
    std::vector<Animation> animations;
    std::size_t currentAnimation = 0;
    std::unique_ptr<ColliderBox> colliderBox;
    bool alive = true;
    bool hasPendingObjToCreate = false;
};
