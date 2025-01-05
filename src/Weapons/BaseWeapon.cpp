#include "Weapons/BaseWeapon.h"

#include "ObjectsManager.h"

#include <math.h>

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

BaseWeapon::BaseWeapon(ObjectsManager & om, int teamId, Projectile baseProjectile)
    : GameObject(om, WeaponBaseVitality, teamId, ObjectType::Weapon)
    , m_baseProjectile(baseProjectile)
{
}

BaseWeapon::~BaseWeapon()
{
    for (auto& projectile : m_projectiles)
    {
        delete projectile;
    }
    m_projectiles.clear();
    m_deleteCandidateProjectiles.clear();
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

    if (m_autoFire)
    {
        m_currentCooldown += dt;
        if (m_currentCooldown >= m_weaponCooldown)
        {
            shoot();
            m_currentCooldown = 0.0f;
        }
    }

    for (auto i = 0; i < m_deleteCandidateProjectiles.size(); i++)
    {
        auto deleteProjectile = m_deleteCandidateProjectiles[i];
        auto it = std::find(m_projectiles.begin(), m_projectiles.end(), deleteProjectile);
        if (it != m_projectiles.end())
        {
            std::swap(m_deleteCandidateProjectiles[i], m_deleteCandidateProjectiles.back());
            m_deleteCandidateProjectiles.pop_back();
            std::swap(*it, m_projectiles.back());
            delete m_projectiles.back();
            m_projectiles.pop_back();
            i--;
        }
    }
    for (auto& projectile : m_projectiles)
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

void BaseWeapon::renderCrosshair(Vector2 Pos) const
{

}

void BaseWeapon::setActive(bool newActive)
{
    m_isActive = newActive;
}

bool BaseWeapon::isActiveWeapon() const
{
    return m_isActive;
}

Texture BaseWeapon::getWeaponTexture() const
{
    return m_texture;
}

void BaseWeapon::shoot()
{
    auto projectile = new Projectile(m_baseProjectile);
    auto pos = center();
    projectile->setPos(pos);

    m_objectManager.getPhysics().createRectangularBody(pos, projectile->texture.height, projectile->texture.width, projectile, true);
    auto velocity = getSpeedToEnemy();
    projectile->setVelocity(velocity);
    projectile->setState(Projectile::State::Alive);
    projectile->onDieSignal.add([this, projectile]() {
       m_deleteCandidateProjectiles.push_back(projectile);
    });
    m_projectiles.push_back(projectile);
}

const Vector2& BaseWeapon::getSpeedToEnemy()
{
    const auto enemies = m_objectManager.getEnemyObjects(m_teamId);
    Vector2 nearestPos = Vector2{ 0,0 };
    Vector2 enemyPos = Vector2{ 0,0 };
    float nearest = MAXFLOAT;
    for (auto& enemie : enemies)
    {
        if (enemie->m_objectType != ObjectType::GravityZone && enemie->m_objectType != ObjectType::LaserProjectile && enemie->m_objectType != ObjectType::RocketProjectile)
        {
            auto distanceVector = enemie->getPos() - m_pos;
            float length = std::sqrt(std::pow(distanceVector.x, 2) + std::pow(distanceVector.y, 2));
            if (length < nearest)
            {
                nearest = length;
                nearestPos = distanceVector;
                enemyPos = enemie->getPos();
            }
        }
    }
    if (nearestPos == Vector2{ 0, 0 })
    {
        return Vector2{ 100, 0};
    }
    return Vector2Scale(nearestPos, (1.0f / nearest) * 300.0f);
}
