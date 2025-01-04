#pragma once

#include "Vitality.h"
#include "Helpers.h"

#include "raylib.h"


// base class for all in-game object like asteroids, enemy ships etc.
class GameObject
{
public:
    GameObject(const VitalityParams& vitality);
    virtual ~GameObject() = default;

    virtual void initialize();

    virtual void update(float dt);
    virtual void render();

    void damage(float damage);
    bool isDead() const;
    virtual void OnDie();

    virtual void reset();

    Vector2 center() const;

    helpers::MulticastDelegate<float, const VitalityData&> OnReceiveDamage;


protected:
    VitalityParams m_vitality;
    VitalityData m_vitalityData;

    Texture m_texture;
    
    // temporary ?
    Vector2 m_pos = { 0.0f, 0.0f };
    Vector2 m_size = { 0.0f, 0.0f };
};

