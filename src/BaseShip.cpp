#include "BaseShip.h"

#include "PlayerStats.h"
#include <cassert>

BaseShip::BaseShip(ObjectsManager &om, const VitalityParams& vitality, int teamId, ObjectType type)
    : GameObject(om, vitality, teamId, type)
{
    m_weaponPlaceTexture = LoadTexture("weaponPlace.png");
    m_weaponPlaceTexture.width = 19;
    m_weaponPlaceTexture.height = 17;
}

void BaseShip::initialize()
{
    GameObject::initialize();
}

void BaseShip::update(float dt)
{
    GameObject::update(dt);
    for (auto i = 0; i < m_weapons.size(); i++)
    {
        auto pos = Vector2{ m_pos.x + m_weaponPositions[i].first, m_pos.y + m_weaponPositions[i].second};

        m_weapons[i]->setPos({ pos.x + 10.0f, pos.y + 7.0f });
        m_weapons[i]->update(dt);
    }
}

void BaseShip::render()
{
    GameObject::render();
    renderWeaponPlaces();
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

            item.params.weaponDamage += m_stageMultipliers.damageMultiplier * PlayerStats::get().m_currDifficulty;

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

void BaseShip::renderWeaponPlaces()
{
    for (auto pos : m_weaponPositions)
    {
        DrawTexture(m_weaponPlaceTexture, m_pos.x + pos.first, m_pos.y + pos.second, WHITE);
    }
}
