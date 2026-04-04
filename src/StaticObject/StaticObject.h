#pragma once
#include "../GameObject/GameObject.h"

class StaticObject : public GameObject
{
public:
    struct Config
    {
        GameObject::Config gameObject;
        bool solid = true;
        bool interactable = false;
    };
    explicit StaticObject(const Config &config);

    void handleInput() override;
    void update(float deltaTime) override;
    void draw() override;

protected:
    bool solid;
    bool interactable;
};