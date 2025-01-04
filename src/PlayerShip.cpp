#include "PlayerShip.h"

#include "external/reasings.h"
#include "Weapons/LaserWeapon.h"

namespace
{
    constexpr VitalityParams PlayerBaseVitality =
    {
        false,
        100.0f,
        true,
        ShieldParams
        {
            100.0f,
            2.0f,
            10.0f
        }
    };

    constexpr float ShakeSpeed = 0.6f;
    constexpr float ShakeRadius = 4.0f;

    Vector2 screenCenter()
    {
        return { GetScreenWidth() * 0.5f, GetScreenHeight() * 0.5f };
    }
}

PlayerShip::PlayerShip(ObjectsManager &om)
    : BaseShip(om, PlayerBaseVitality, 0, ObjectType::PlayerShip)
{
    m_texture = LoadTexture("temp-spaceships/klaed_base.png");
    m_weapons.push_back(new LaserWeapon(om, getTeamId()));
}

void PlayerShip::initialize()
{
    BaseShip::initialize();
    reset();
}

void PlayerShip::update(float dt)
{
    BaseShip::update(dt);

    m_offsetProgress += ShakeSpeed * dt;

    while (m_offsetProgress > 2.0f)
    {
        m_offsetProgress -= 2.0f;
    };

    const float progressToCalculate = m_offsetProgress < 1.0f ? m_offsetProgress : 2.0f - m_offsetProgress;

    m_pos.y = screenCenter().y + EaseSineInOut(progressToCalculate, 1.0f, 1.0f, 1.0f) * ShakeRadius;
}

void PlayerShip::reset()
{
    BaseShip::reset();

    m_pos = screenCenter();
    m_offsetProgress = 0.0f;
}

void PlayerShip::render()
{
    BaseShip::render();
}
