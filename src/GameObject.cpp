// by Slava

#include "GameObject.h"
#include "ObjectsManager.h"
#include "PlayerStats.h"

#include "utils.h"
#include <algorithm>

GameObject::GameObject(ObjectsManager &objectManager, const VitalityParams& vitality, int teamId, ObjectType type)
    : m_objectManager(objectManager)
    , m_vitality(vitality)
    , m_vitalityData(vitality)
    , m_teamId(teamId)
    , m_objectType(type)
{
    m_objectManager.addObject(this);
}

GameObject::~GameObject()
{
    m_objectManager.deleteObject(this);
}

void GameObject::initialize()
{
    m_size = { static_cast<float>(m_texture.width), static_cast<float>(m_texture.height) };
}

void GameObject::update(float dt)
{
    m_noDamageTime += dt;
    if (m_noDamageTime >= m_vitality.shieldParams.startRegenDelay && m_vitalityData.currentShield < m_vitality.shieldParams.maxShield)
    {
        m_vitalityData.currentShield = std::clamp(m_vitalityData.currentShield + m_vitality.shieldParams.regenPerSec * dt, 0.0f, m_vitality.shieldParams.maxShield);
    }
}

void GameObject::render()
{
    DrawTextureV(m_texture, center() + m_renderOffset, WHITE);
}

void GameObject::damage(float damage)
{
    if (!m_vitality.isImmortal)
    {
        m_noDamageTime = 0.0f;
        if (m_vitality.hasShield)
        {
            auto curShield = m_vitalityData.currentShield - damage;
            auto curDamage = damage - m_vitalityData.currentShield;
            m_vitalityData.currentShield = curShield < 0 ? 0 : curShield;
            damage = curDamage < 0 ? 0 : curDamage;
        }
        m_vitalityData.currentHP -= damage;

        if (isDead())
        {
            auto& stats = PlayerStats::get();
            stats.addXP(m_xpValue);
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

    return m_vitalityData.currentHP <= 0.0f;
}

void GameObject::OnDie()
{
    onDieSignal.broadcast();
}

void GameObject::reset()
{
}

Vector2 GameObject::center() const
{
    return { m_pos.x - m_size.x * 0.5f, m_pos.y - m_size.y * 0.5f };
}

void GameObject::setPos(const Vector2 &pos)
{
    m_pos = pos;
}

const Vector2& GameObject::getPos() const
{
    return m_pos;
}

void GameObject::setRotation(float rot)
{
    m_rotation = rot;
}

float GameObject::getRotation() const
{
    return m_rotation;
}

void GameObject::setRenderOffset(const Vector2 &offset)
{
    m_renderOffset = offset;
}

int GameObject::getTeamId() const
{
    return m_teamId;
}

void GameObject::setPhysicsComp(PhysicsComp *physicsComp)
{
    m_physicsComp = physicsComp;
}

void GameObject::onCollision(GameObject *other)
{
    // void by default?
}

float GameObject::getCurrentHP() const
{
    return m_vitalityData.currentHP;
}
