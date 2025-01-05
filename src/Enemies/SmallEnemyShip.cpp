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
        addWeapon(new LaserWeapon(m_om, getTeamId()));
        break;

    case 1:
        addWeapon(new RocketWeapon(m_om, getTeamId()));
        break;

    default:
        addWeapon(new LaserWeapon(m_om, getTeamId()));
        break;
    }
}
