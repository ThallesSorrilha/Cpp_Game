#pragma once

#include <memory>

#include "../GameObject/GameObject.h"
#include "../ColliderBox/ColliderBox.h"

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

protected:
    std::unique_ptr<ColliderBox> colliderBox;
};
