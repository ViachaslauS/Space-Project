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

    constexpr float BaseAsteroidDamage = 200.0f;
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
        obj->damage(obj->getCurrentHP());
        damage(BaseAsteroidDamage);
    }
}