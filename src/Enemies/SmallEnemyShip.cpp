#include "Enemies/SmallEnemyShip.h"

#include "raylib.h"
#include "Weapons/LaserWeapon.h"
#include "Weapons/RocketWeapon.h"

SmallEnemyShip::SmallEnemyShip(ObjectsManager &om)
    : BaseEnemyShip(om)
    , m_om(om)
{
    m_texture = LoadTexture("temp-spaceships/klaed_base.png");
    m_xpValue = 200.0f;
}

void SmallEnemyShip::initialize()
{
    BaseEnemyShip::initialize();
    const auto weaponType = GetRandomValue(0, static_cast<int>(WeaponType::Count) - 1);
    switch (weaponType)
    {
    case 0:
        m_weapons.push_back(new LaserWeapon(m_om, getTeamId()));
        break;

    case 1:
        m_weapons.push_back(new RocketWeapon(m_om, getTeamId()));
        break;

    default:
        break;
    }
}
