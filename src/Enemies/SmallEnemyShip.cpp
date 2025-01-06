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

    constexpr VitalityParams BaseVitality
    {
        false,
        5.0f,
        true,
        ShieldParams
        {
            2.0f,
            0.0f,
            0.0f
        }
    };

    constexpr StageMultipliers SmallShipMuls
    {
        .hpMultiplier = 1.0f,
        .shieldMultiplier = 0.0f,
        .damageMultiplier = 3.0f
    };
}

SmallEnemyShip::SmallEnemyShip(ObjectsManager &om)
    : BaseEnemyShip(om, BaseVitality, SmallShipMuls)
    , m_om(om)
{
    m_texture = LoadTexture("smallEnemyShip.png");
    m_texture.width = 300;
    m_texture.height = 80;
    m_xpValue = 2.0f;

    m_stageMultipliers.damageMultiplier = 1.0f;
    m_stageMultipliers.hpMultiplier = 2.0f;

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
