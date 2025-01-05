#include "Enemies/SmallEnemyShip.h"

#include "raylib.h"
#include "Weapons/LaserWeapon.h"
#include "Weapons/RocketWeapon.h"

namespace
{
    std::vector<std::pair<int,int>> WeaponPos = 
    {
        { -15, -50 }
    };
}

SmallEnemyShip::SmallEnemyShip(ObjectsManager &om)
    : BaseEnemyShip(om)
    , m_om(om)
{
    m_texture = LoadTexture("smallEnemyShip.png");
    m_texture.width = 300;
    m_texture.height = 80;
    m_xpValue = 200.0f;

    m_weaponPositions = WeaponPos;
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
