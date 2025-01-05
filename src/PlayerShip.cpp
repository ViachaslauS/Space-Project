#include "PlayerShip.h"

#include "external/reasings.h"
#include "Weapons/LaserWeapon.h"
#include "Weapons/Gravigun.hpp"

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
}

PlayerShip::PlayerShip(ObjectsManager &om, GravityZoneSystem &gz)
    : BaseShip(om, PlayerBaseVitality, 0, ObjectType::PlayerShip)
    , m_gravityZones(gz)
{
    m_texture = LoadTexture("temp-spaceships/klaed_base.png");
    m_maxWeaponCount = 5;

    addWeapon(new Gravigun(om, getTeamId(), m_gravityZones));
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

    m_pos.y = helpers::windowCenter().y + EaseSineInOut(progressToCalculate, 1.0f, 1.0f, 1.0f) * ShakeRadius;
}

void PlayerShip::reset()
{
    BaseShip::reset();

    m_pos = helpers::windowCenter();
    m_offsetProgress = 0.0f;
}

void PlayerShip::render()
{
    BaseShip::render();
}

void PlayerShip::onCollision(GameObject *other)
{
    switch (other->m_objectType)
    case ObjectType::Asteroid: {
        other->damage(other->getCurrentHP());
        damage(5);
        break;
    }
}
