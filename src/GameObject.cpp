// by Slava

#include "GameObject.h"

GameObject::GameObject(const VitalityParams& vitality)
    : m_vitality(vitality)
    , m_vitalityData(vitality)
{
}

void GameObject::initialize()
{

}

void GameObject::update(float dt)
{

}

void GameObject::render()
{
    DrawTextureV(m_texture, center(), WHITE);
}

void GameObject::damage(float damage)
{
    if (!m_vitality.isImmortal)
    {
        m_vitalityData.currentHP -= damage;

        if (isDead())
        {
            OnDie();
        }
    }

    OnReceiveDamage.broadcast(m_vitality.isImmortal ? 0.0f : damage, m_vitalityData);
}

bool GameObject::isDead() const
{
    if (m_vitality.isImmortal)
    {
        return false;
    }

    return m_vitalityData.currentHP > 0.0f;
}

void GameObject::OnDie()
{
}

void GameObject::reset()
{

}

Vector2 GameObject::center() const
{
    return { m_pos.x - m_size.x * 0.5f, m_pos.y - m_size.y * 0.5f };
}
