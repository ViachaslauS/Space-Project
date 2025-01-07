#include "Projectile.h"

namespace
{
constexpr VitalityParams ProjectileBaseVitality =
    {
        false,
        1.0f,
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

Projectile::Projectile(ObjectsManager &om, int teamId, ObjectType type)
    :GameObject(om, ProjectileBaseVitality, teamId, type)
{
}

Projectile::Projectile(const Projectile& proj)
    :GameObject(proj.m_objectManager, ProjectileBaseVitality, proj.m_teamId, proj.m_objectType)
{
    texture = proj.texture;
    damage = proj.damage;
}

Projectile::State Projectile::getState() {
    return state;
}

void Projectile::setState(Projectile::State newState)
{
    state = newState;
}

void Projectile::update(float dt)
{
    if (state == State::Alive)
    {
        if (helpers::isInWindow(m_pos) == false)
        {
            OnDie();
        }
    }
}

void Projectile::render()
{
    if (state == State::Alive)
    {
        Vector2 drawPos = m_pos;

        DrawTextureEx(texture, drawPos, m_rotation * (180 / PI), 1.0f, WHITE);

    }
}

void Projectile::setVelocity(const Vector2& velocity)
{
    m_physicsComp->physics->setVelocityWithRotation(m_physicsComp, velocity);
}

void Projectile::onCollision(GameObject* obj)
{
    if (obj->getTeamId() != m_teamId)
    {
        obj->damage(damage);
        OnDie();
    }
}
