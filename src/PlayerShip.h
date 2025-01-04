#pragma once

#include "GameObject.h"
#include "BaseShip.h"

class PlayerShip : public BaseShip
{
public:
    PlayerShip();

    virtual void initialize() override;
    virtual void update(float dt) override;
    virtual void reset() override;
    virtual void render() override;

protected:
    float m_offsetProgress = 0.0f;
};