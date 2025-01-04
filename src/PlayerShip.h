#pragma once

#include "GameObject.h"
#include "BaseShip.h"

class ObjectsManager;

class PlayerShip : public BaseShip
{
public:
    PlayerShip(ObjectsManager &om);

    virtual void initialize() override;
    virtual void update(float dt) override;
    virtual void reset() override;
    virtual void render() override;

protected:
    float m_offsetProgress = 0.0f;
};
