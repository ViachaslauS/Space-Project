#include "Enemies/SmallEnemyShip.h"

#include "raylib.h"
#include "Weapons/LaserWeapon.h"
#include "Weapons/RocketWeapon.h"

SmallEnemyShip::SmallEnemyShip()
    :BaseEnemyShip()
{
    m_texture = LoadTexture("temp-spaceships/klaed_base.png");
}

void SmallEnemyShip::initialize()
{
    BaseEnemyShip::initialize();
    const auto weaponType = GetRandomValue(0, static_cast<int>(WeaponType::Count) - 1);
    switch (weaponType)
    {
    case 0:
        m_weapons.push_back(new LaserWeapon(getTeamId()));
        break;

    case 1:
        m_weapons.push_back(new RocketWeapon(getTeamId()));
        break;

    default:
        break;
    }
}