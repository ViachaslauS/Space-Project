#include "Asteroid.h"
#include "Physics.hpp"

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

Asteroid::Asteroid(ObjectsManager &om)
    : GameObject(om, AsteroidBaseVitality, 2, ObjectType::Asteroid)
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
}

void Asteroid::setVelocity(const Vector2 &velocity)
{
    m_physicsComp->physics->setVelocity(m_physicsComp, velocity);
}
