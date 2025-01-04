#include "Weapons/BaseWeapon.h"

namespace
{
    constexpr VitalityParams WeaponBaseVitality =
    {
        false,
        10.0f,
        false,
        ShieldParams
        {
            0.0f,
            0.0f,
            0.0f
        }
    };
}

BaseWeapon::BaseWeapon(int teamId, ObjectType type)
    : GameObject(WeaponBaseVitality, teamId, type)
{
}

const Vector2& BaseWeapon::getPos() const
{
    return m_pos;
}

void BaseWeapon::setPos(const Vector2& pos)
{
    m_pos = pos;
}

void BaseWeapon::update(float dt)
{
    GameObject::update(dt);
    m_currentCooldown += dt;
    if (m_currentCooldown >= m_weaponCooldown)
    {
        shoot();
        m_currentCooldown = 0.0f;
    }

    for (auto projectile : m_projectiles)
    {
        projectile->update(dt);
    }
}

void BaseWeapon::render()
{
    DrawTexture(m_texture, m_pos.x, m_pos.y, WHITE);
    for (auto projectile : m_projectiles)
    {
        projectile->render();
    }
}

void BaseWeapon::shoot()
{
    Projectile* projectile = nullptr;

    auto it = std::find_if(m_projectiles.begin(), m_projectiles.end(), [](const auto projectile) {
        return projectile->getState() == Projectile::State::Unused;
    });
    if (it == m_projectiles.end())
    {
        projectile = new Projectile(m_baseProjectile);
    }
    else
    {
        projectile = *it;
    }
    projectile->pos = center();
    projectile->speed = Vector2{ 100, 0 };
    projectile->setState(Projectile::State::Alive);
    m_projectiles.push_back(projectile);
}

const Vector2& BaseWeapon::getNearestEnemyPosition() const
{
    return Vector2();
}
