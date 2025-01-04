#pragma once

#include "GameObject.h"

class Asteroid : public GameObject
{
public:
    Asteroid();

public:
    virtual void update(float dt) override;

    virtual void initialize() override;

    void setSpeed(Vector2 speed)
    {
        m_speed = speed;
    }

    void setPosition(Vector2 pos)
    {
        m_pos = pos;
    }

protected:
    Vector2 m_speed;
};