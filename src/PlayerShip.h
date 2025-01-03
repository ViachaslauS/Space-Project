#pragma once

#include "GameObject.h"

class PlayerShip : public GameObject
{
public:
    PlayerShip();

    virtual void initialize() override;
    virtual void update(float dt) override;
    virtual void reset() override;
};