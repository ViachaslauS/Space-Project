#include "Enemies/BaseEnemyShip.h"

#include "raymath.h"
namespace
{
     constexpr VitalityParams EnemyBaseVitality =
    {
        false,
        100.0f,
        true,
        ShieldParams
        {
            100.0f,
            2.0f,
            10.0f
        }
    };

    constexpr float BaseAsteroidDamage = 20.0f;
}

BaseEnemyShip::BaseEnemyShip(ObjectsManager &om)
    : BaseShip(om, EnemyBaseVitality, 1, ObjectType::EnemyShip)
{
}

void BaseEnemyShip::initialize()
{
    BaseShip::initialize();
}

void BaseEnemyShip::update(float dt)
{
    BaseShip::update(dt);

    auto phys = m_physicsComp->physics;
    auto vel = phys->getVelocity(m_physicsComp);

    if (vel.y > 0.5f) {
        phys->applyForce(m_physicsComp, { 0.0f, -m_thrust });
    } else if (vel.y < -0.5f) {
        phys->applyForce(m_physicsComp, { 0.0f, m_thrust });
    }

    if (vel.x < 10.0f) {
        phys->applyForce(m_physicsComp, { m_thrust, 0.0f });
    }

    if (vel.x > 40.0f) {
        phys->applyForce(m_physicsComp, { -m_thrust, 0.0f });
    }
}

void BaseEnemyShip::render()
{
    BaseShip::render();
}

void BaseEnemyShip::setVelocity(const Vector2& velocity)
{
    m_physicsComp->physics->setVelocity(m_physicsComp, velocity);
}

void BaseEnemyShip::onCollision(GameObject* obj)
{
    if (obj->m_objectType == ObjectType::Asteroid)
    {
        //obj->damage(obj->getCurrentHP());
        damage(BaseAsteroidDamage);
    }
}
