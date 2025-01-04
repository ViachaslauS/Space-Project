#pragma once

#include "BaseShip.h"

class ObjectsManager;

class BaseEnemyShip : public BaseShip
{
public:
    BaseEnemyShip(ObjectsManager &om);

    virtual void initialize() override;

    virtual void update(float dt) override;
    virtual void render() override;

    void setSpeed(Vector2 speed)
    {
        m_speed = speed;
    }

protected:
    Vector2 m_speed = Vector2{ 0, 0};
};
