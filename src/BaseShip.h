#pragma once

#include <memory>

#include "GameObject.h"
#include "Weapons/BaseWeapon.h"
#include <cassert>

#include <utility>
class ObjectsManager;

// for enemies
struct StageMultipliers
{
    float hpMultiplier = 0.0f;
    float shieldMultiplier = 0.0f;
    float damageMultiplier = 0.0f;
};

struct WeaponParamMap
{
    WeaponType weaponType;
    BaseWeapon::WeaponParam params;
};

inline const std::vector<WeaponParamMap> DefaultWeaponParamsMap =
{
    { WeaponType::Gravigun, { 2.0f, 2.0f }},
    { WeaponType::Laser, { 1.0f, 1.0f }},
    { WeaponType::RocketLauncher, { 2.5, 10.0f }}
};

inline BaseWeapon::WeaponParam GetDefaultParam(WeaponType weaponType)
{
    for (auto& param : DefaultWeaponParamsMap)
    {
        if (param.weaponType == weaponType)
        {
            return param.params;
        }
    }

    assert(false && "what are you looking for, huh?");
    return {};
}

class BaseShip : public GameObject
{
public:
    BaseShip(ObjectsManager &om, const VitalityParams& vitality, int teamId, ObjectType type);

    virtual void initialize() override;
    virtual void update(float dt) override;
    virtual void reset() override;
    virtual void render() override;
    virtual void onCollision(GameObject* obj) override;

    virtual void renderWeaponPlaces();

    void addWeapon(BaseWeapon* newWeapon);
    int getMaxWeapons() const;

    void setPosition(Vector2 pos)
    {
        m_pos = pos;
    }

    const std::vector<std::unique_ptr<BaseWeapon>>& getWeapons() const;

    void applyWeaponParam(WeaponType weaponType, BaseWeapon::WeaponParam newParam);

protected:
    void updateAllWeaponParams();
    BaseWeapon::WeaponParam getParamForWeaponType(WeaponType weaponType) const;

protected:
    int m_maxWeaponCount = 1;
    std::vector<std::unique_ptr<BaseWeapon>> m_weapons;

    std::vector<WeaponParamMap> m_weaponParams = DefaultWeaponParamsMap;
    std::vector<std::pair<int,int>> m_weaponPositions;
    Texture m_weaponPlaceTexture;

    StageMultipliers m_stageMultipliers;
};
