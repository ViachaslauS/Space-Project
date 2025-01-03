// by Slava

#pragma once

#include "Vitality.h"

#include "raylib.h"


// base class for all in-game object like asteroids, enemy ships etc.
class GameObject
{
public:
    GameObject(const VitalityParams& vitality);

    virtual void update(float dt);
    virtual void render();

    bool isDead() const;

    virtual void OnDie();

protected:
    VitalityParams m_vitality;
    VitalityData m_vitalityData;

    Texture m_texture;
    
    // temporary ?
    Vector2 m_pos;
};

