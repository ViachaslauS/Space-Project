#include "Weapons/RocketWeapon.h"

RocketWeapon::RocketWeapon(int teamId)
    : BaseWeapon(teamId, ObjectType::RocketProjectile)
{
    auto rocketTexture = LoadTexture("rocket.png");
    rocketTexture.height = 50;
    rocketTexture.width = 50;
    m_weaponCooldown = 4.0f;
    //Configure bullet params
    m_baseProjectile = {
        rocketTexture,
        Vector2{0,0},
        Vector2{0,0},
        100.0f,
        teamId,
        Projectile::State::Unused,
    };
}

void RocketWeapon::update(float dt)
{
    BaseWeapon::update(dt);
}
