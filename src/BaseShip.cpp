#include "BaseShip.h"

BaseShip::BaseShip(ObjectsManager &om, const VitalityParams& vitality, int teamId, ObjectType type)
    : GameObject(om, vitality, teamId, type)
{
}

void BaseShip::initialize()
{
    GameObject::initialize();
}

void BaseShip::update(float dt)
{
    GameObject::update(dt);
    for (auto weapon : m_weapons)
    {
        weapon->setPos(m_pos);
        weapon->update(dt);
    }
}

void BaseShip::render()
{
    GameObject::render();
    for (auto weapon : m_weapons)
    {
        weapon->render();
    }
}

const std::vector<BaseWeapon*>& BaseShip::getWeapons() const
{
    return m_weapons;
}

void BaseShip::reset()
{
    GameObject::reset();
}
