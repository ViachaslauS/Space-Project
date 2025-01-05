// by Slava

#include "GameObject.h"
#include "ObjectsManager.h"
#include "PlayerStats.h"

#include "utils.h"

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

void GameObject::onSensorCollision(GameObject *other, bool exit)
{
}

float GameObject::getCurrentHP() const
{
    return m_vitalityData.currentHP;
}
