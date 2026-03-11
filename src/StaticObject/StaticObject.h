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

    bool init() override;
    void handleInput() override;
    void update(float deltaTime) override;
    void draw() override;
    void shutdown() override;
protected:
    bool solid;
    bool interactable;
};