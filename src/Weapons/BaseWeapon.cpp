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

void BaseWeapon::applyParams(WeaponParam newParams)
{
    m_params = newParams;
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
        m_currCooldown += dt;
        if (m_currCooldown >= m_params.weaponCooldown)
        {
            shoot();
            m_currCooldown = 0.0f;
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

WeaponType BaseWeapon::getWeaponType() const
{
    return m_weaponType;
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

const Vector2 BaseWeapon::getSpeedToEnemy()
{
    const auto enemies = m_objectManager.getEnemyObjects(m_teamId);
    Vector2 nearestPos = Vector2{ 0,0 };
    float nearest = std::numeric_limits<float>::max();
    for (auto& enemy : enemies)
    {
        if (enemy->m_objectType != ObjectType::GravityZone && enemy->m_objectType != ObjectType::LaserProjectile && enemy->m_objectType != ObjectType::RocketProjectile)
        {
            auto distanceVector = enemy->getPos() - m_pos;
            float length = std::sqrt(std::pow(distanceVector.x, 2) + std::pow(distanceVector.y, 2));
            if (length < nearest)
            {
                nearest = length;
                nearestPos = distanceVector;
            }
        }
    }
    if (nearestPos == Vector2{ 0, 0 })
    {
        return Vector2{ 100, 0};
    }

    nearestPos.x += helpers::randFlt(-150.0f, 150.0f);
    nearestPos.y += helpers::randFlt(-150.0f, 150.0f);
    return Vector2Scale(nearestPos, (1.0f / nearest) * 300.0f);
}
