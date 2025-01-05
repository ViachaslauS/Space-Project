#include "BaseShip.h"
#include <cassert>

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
    for (auto& weapon : m_weapons)
    {
        weapon->setPos(m_pos);
        weapon->update(dt);
    }
}

void BaseShip::render()
{
    GameObject::render();
    for (auto& weapon : m_weapons)
    {
        weapon->render();
    }
}

void BaseShip::addWeapon(BaseWeapon* newWeapon)
{
    if (m_weapons.size() < m_maxWeaponCount)
    {
        newWeapon->applyParams(getParamForWeaponType(newWeapon->getWeaponType()));
        m_weapons.push_back(std::unique_ptr<BaseWeapon>(newWeapon));
    }
}

int BaseShip::getMaxWeapons() const
{
    return m_maxWeaponCount;
}

const std::vector<std::unique_ptr<BaseWeapon>>& BaseShip::getWeapons() const
{
    return m_weapons;
}

void BaseShip::applyWeaponParam(WeaponType weaponType, BaseWeapon::WeaponParam newParam)
{
    for (auto& item : m_weaponParams)
    {
        if (item.weaponType == weaponType)
        {
            item.params = newParam;
            break;
        }
    }

    updateAllWeaponParams();
}

void BaseShip::updateAllWeaponParams()
{
    for (auto& weapon : m_weapons)
    {
        weapon->applyParams(getParamForWeaponType(weapon->getWeaponType()));
    }
}

BaseWeapon::WeaponParam BaseShip::getParamForWeaponType(WeaponType weaponType) const
{
    for (auto& item : m_weaponParams)
    {
        if (item.weaponType == weaponType)
        {
            return item.params;
        }
    }

    assert(false && "not defined default params for weapon type");

    return {};
}

void BaseShip::reset()
{
    GameObject::reset();
}

void BaseShip::onCollision(GameObject* obj)
{
    GameObject::onCollision(obj);
}