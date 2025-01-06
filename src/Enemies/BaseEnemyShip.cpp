#include "Enemies/BaseEnemyShip.h"

#include "raymath.h"
#include <PlayerStats.h>
namespace
{
     constexpr VitalityParams EnemyBaseVitality =
        {
            false,
            5.0f,
            true,
            ShieldParams
            {
                2.0f,
                0.0f,
                0.0f
            }
        };

     constexpr StageMultipliers Multipliers
     {
         .hpMultiplier = 2.0f,
         .shieldMultiplier = 3.0f,
         .damageMultiplier = 2.5f
     };

     VitalityParams calculateEnemyBaseVitality(const VitalityParams& originParams, const StageMultipliers& multipliers)
     {
         auto outVitality = originParams;
         auto currDiff = PlayerStats::get().m_currDifficulty;

         outVitality.maxHp += Multipliers.hpMultiplier * currDiff;
         outVitality.shieldParams.maxShield += Multipliers.shieldMultiplier * currDiff;

         return outVitality;
     }

    constexpr float BaseAsteroidDamage = 20.0f;
}

BaseEnemyShip::BaseEnemyShip(ObjectsManager &om, const VitalityParams& baseVitality, const StageMultipliers& multipliers)
    : BaseShip(om, calculateEnemyBaseVitality(baseVitality, multipliers), 1, ObjectType::EnemyShip)
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

    Vector2 force { 0.0f, 0.0f };
    if (vel.y > 0.5f) {
        force.y = -m_thrust;
    } else if (vel.y < -0.5f) {
        force.y = m_thrust;
    }

    if (vel.x < 10.0f) {
        force.x = m_thrust;
    } else if (vel.x > 60.0f) {
        force.x = -m_thrust;
    }

    if (force.x != 0.0f || force.y != 0.0f) {
        phys->applyForce(m_physicsComp, force);
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
        obj->damage(obj->getCurrentHP());
        damage(BaseAsteroidDamage);
    }

    if (obj->m_objectType == ObjectType::LaserProjectile)
    {
        auto lp = static_cast<Projectile *>(obj);
        damage(lp->damage);
        lp->GameObject::damage(lp->getCurrentHP());
    }

    if (obj->m_objectType == ObjectType::RocketProjectile)
    {
        auto lp = static_cast<Projectile *>(obj);
        damage(lp->damage);
        lp->GameObject::damage(lp->getCurrentHP());
    }
}
