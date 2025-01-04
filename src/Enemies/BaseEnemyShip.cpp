#include "Enemies/BaseEnemyShip.h"

#include "raymath.h"
namespace
{
     constexpr VitalityParams EnemyBaseVitality =
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
}

BaseEnemyShip::BaseEnemyShip()
    : BaseShip(EnemyBaseVitality, 1, ObjectType::EnemyShip)
{
}

void BaseEnemyShip::initialize()
{
    BaseShip::initialize();
}

void BaseEnemyShip::update(float dt)
{
    BaseShip::update(dt);
    m_pos += Vector2Scale(m_speed, dt);
}

void BaseEnemyShip::render()
{
    BaseShip::render();
}
