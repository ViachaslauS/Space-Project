#include "Asteroid.h"

#include "raymath.h"

namespace
{
     constexpr VitalityParams AsteroidBaseVitality = 
    {
        false,
        20.0f,
        false,
        ShieldParams 
        {
            0.0f,
            0.0f,
            0.0f
        }
    };
}

Asteroid::Asteroid()
    : GameObject(AsteroidBaseVitality, 2)
{
    m_texture = LoadTexture("Asteroid.png");
}

void Asteroid::initialize()
{
    GameObject::initialize();
}

void Asteroid::update(float dt)
{
    GameObject::update(dt);
    m_pos += Vector2Scale(m_speed, dt);
}