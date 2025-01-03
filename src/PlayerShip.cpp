#include "PlayerShip.h"

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
}

PlayerShip::PlayerShip()
    : GameObject(PlayerBaseVitality)
{
    m_texture = LoadTexture("temp-spaceships/klaed_base.png");
}

void PlayerShip::initialize()
{
    reset();

    m_size = { static_cast<float>(m_texture.width), static_cast<float>(m_texture.height) };
}

void PlayerShip::update(float dt)
{
    GameObject::update(dt);

}

void PlayerShip::reset()
{
    GameObject::reset();

    m_pos = { GetScreenWidth() * 0.5f, GetScreenHeight() * 0.5f };
}
