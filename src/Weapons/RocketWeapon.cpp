#include "Weapons/RocketWeapon.h"

RocketWeapon::RocketWeapon(ObjectsManager &om, int teamId)
    : BaseWeapon(om, teamId, Projectile(om, teamId, ObjectType::RocketProjectile))
{
    auto rocketTexture = LoadTexture("rocket.png");
    rocketTexture.height = 50;
    rocketTexture.width = 50;
    m_weaponCooldown = 4.0f;
    //Configure bullet params
    m_baseProjectile.texture = rocketTexture;
    m_baseProjectile.damage = 200.0f;
}

void RocketWeapon::update(float dt)
{
    BaseWeapon::update(dt);
}
