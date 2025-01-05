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
    constexpr float AsteroidDamage = 20.0f;
}

Asteroid::Asteroid(ObjectsManager &om)
    : GameObject(om, AsteroidBaseVitality, 2, ObjectType::Asteroid)
{
    m_texture = LoadTexture("Asteroid.png");
    m_xpValue = 100.0f;
}

void Asteroid::initialize()
{
    GameObject::initialize();
}

void Asteroid::update(float dt)
{
    GameObject::update(dt);
    m_lifeTime -= dt;
    if (m_lifeTime <= 0.0f)
    {
        OnDie();    
    }
}

void Asteroid::setVelocity(const Vector2 &velocity)
{
    m_physicsComp->physics->setVelocity(m_physicsComp, velocity);
}

void Asteroid::onCollision(GameObject* other)
{
    if (other->m_objectType == ObjectType::Asteroid)
    {
        return;
    }
}