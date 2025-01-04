#include "Weapons/LaserWeapon.h"

LaserWeapon::LaserWeapon(int teamId)
    : BaseWeapon(teamId, ObjectType::LaserProjectile)
{
    auto laserTexture = LoadTexture("laser.png");
    laserTexture.height = 50;
    laserTexture.width = 50;
    m_weaponCooldown = 2.0f;

    //Configure bullet params
    m_baseProjectile = {
        laserTexture,
        Vector2{0,0},
        Vector2{0,0},
        20.0f,
        teamId,
        Projectile::State::Unused,
    };
}

void LaserWeapon::update(float dt)
{
    BaseWeapon::update(dt);
}
