#pragma once

#include "GameObject.h"

class Asteroid : public GameObject
{
public:
    Asteroid(ObjectsManager &om);

public:
    virtual void update(float dt) override;

    virtual void initialize() override;

    void setVelocity(const Vector2 &velocity);
};
