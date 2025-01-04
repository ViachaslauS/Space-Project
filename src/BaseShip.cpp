#include "BaseShip.h"

BaseShip::BaseShip(const VitalityParams& vitality, int teamId)
    : GameObject(vitality, teamId)
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

void BaseShip::reset()
{
    GameObject::reset();
}