#include "Weapons/BaseWeapon.h"

#include "ObjectsManager.h"

#include "rlgl.h"

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
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

    m_baseProjectile.damage = newParams.weaponDamage;
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

    m_currCooldown += dt;

    if (m_autoFire)
    {
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
    auto rot = m_weaponAngle * (180.0f / M_PI);
    auto pos = center() + m_renderOffset;

    Rectangle source = { 0.0f, 0.0f, (float)m_texture.width, (float)m_texture.height };
    Rectangle dest = { pos.x, pos.y, (float)m_texture.width, (float)m_texture.height };

    Vector2 origin = { 15.0f, m_texture.height * 0.5f };

    DrawTexturePro(m_texture, source, dest, origin, rot, WHITE);

    for (auto projectile : m_projectiles)
    {
        projectile->render();
    }
}

void BaseWeapon::shoot()
{
    auto projectile = new Projectile(m_baseProjectile);
    auto w = projectile->texture.width;
    auto h = projectile->texture.height;
    auto w2 = projectile->texture.width * 0.5f;
    auto h2 = projectile->texture.height * 0.5f;
    auto pos = center() + Vector2 { 0.0f, -m_texture.height * 0.5f };
    auto r = projectile->texture.height * 0.15f;
    Vector2 c1 { r + 0.25f * w, 3.0f * r };
    Vector2 c2 { r + w2, 3.0f * r };

    projectile->setPos(pos);

    m_objectManager.getPhysics().createCapsuleBody(pos, c1, c2, r, projectile, true);
    auto velocity = getSpeedToEnemy();
    projectile->setVelocity(velocity);
    projectile->setState(Projectile::State::Alive);
    projectile->onDieSignal.add([this, projectile]() {
       m_deleteCandidateProjectiles.push_back(projectile);
    });

    float angle = helpers::vecToAngle(velocity);
    projectile->setRotation(angle);
    m_projectiles.push_back(projectile);
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

bool BaseWeapon::isManualControlAvailable() const
{
    return m_isManualControlAvailable;
}

Texture BaseWeapon::getWeaponTexture() const
{
    return m_texture;
}

Texture BaseWeapon::getWeaponIcon() const
{
    return m_iconTexture;
}

WeaponType BaseWeapon::getWeaponType() const
{
    return m_weaponType;
}

bool BaseWeapon::canShoot() const
{
    return m_currCooldown >= m_params.weaponCooldown;
}

const Vector2 BaseWeapon::getSpeedToEnemy()
{
    const auto enemies = m_objectManager.getEnemyObjects(m_teamId);
    Vector2 nearestPos = Vector2{ 0,0 };
    Vector2 dirToEnemy = Vector2{ 0,0 };
    float nearest = std::numeric_limits<float>::max();
    bool nearestFound = false;
    for (auto& enemy : enemies)
    {
        if (enemy->m_objectType != ObjectType::GravityZone
            && enemy->m_objectType != ObjectType::LaserProjectile
            && enemy->m_objectType != ObjectType::RocketProjectile)
        {
            auto dir = enemy->center() - m_pos;
            float length = Vector2Length(dir);
            if (length < nearest)
            {
                nearestFound = true;
                nearest = length;
                nearestPos = enemy->center();
                dirToEnemy = dir;
            }
        }
    }

    if (!nearestFound)
    {
        nearest = 1;
        auto dir = Vector2Rotate({ 1.0f, 0.0f }, RAD2DEG * m_weaponAngle);
        return Vector2Scale(dir, 300.0);
    }

    dirToEnemy.x += helpers::randFlt(-150.0f, 150.0f);
    dirToEnemy.y += helpers::randFlt(-150.0f, 150.0f);

    calculateDirAngle(Vector2Subtract(m_pos + dirToEnemy, m_pos));

    return Vector2Scale(dirToEnemy, (1.0f / nearest) * 300.0f);
}

void BaseWeapon::calculateDirAngle(const Vector2 dir)
{
    m_weaponAngle = helpers::vecToAngle(dir);
}
