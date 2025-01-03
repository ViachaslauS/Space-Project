// by Slava

#include "GameObject.h"

GameObject::GameObject(const VitalityParams& vitality)
    : m_vitality(vitality)
    , m_vitalityData(vitality)
{
    m_texture = LoadTexture("temp-spaceships/klaed_base.png");
    m_pos = { 300.0f, 300.0f };
}

void GameObject::update(float dt)
{

}

void GameObject::render()
{
    DrawTexture(m_texture, m_pos.x, m_pos.y, WHITE);
}

bool GameObject::isDead() const
{
    if (m_vitality.bImmortal)
    {
        return false;
    }

    return m_vitalityData.currentHP > 0.0f;
}

void GameObject::OnDie()
{
}
