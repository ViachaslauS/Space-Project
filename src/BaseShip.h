#pragma once

#include <memory>

#include "GameObject.h"
#include "Weapons/BaseWeapon.h"

class ObjectsManager;

enum class WeaponType
{
    LaserWeapon,
    RocketWeapon,
    GraviGun,

    Count,
};

class BaseShip : public GameObject
{
public:
    BaseShip(ObjectsManager &om, const VitalityParams& vitality, int teamId, ObjectType type);

    virtual void initialize() override;
    virtual void update(float dt) override;
    virtual void reset() override;
    virtual void render() override;
    virtual void onCollision(GameObject* obj) override;

    void addWeapon(BaseWeapon* newWeapon);
    int getMaxWeapons() const;

    void setPosition(Vector2 pos)
    {
        m_pos = pos;
    }

    const std::vector<std::unique_ptr<BaseWeapon>>& getWeapons() const;

protected:
    int m_maxWeaponCount = 1;
    std::vector<std::unique_ptr<BaseWeapon>> m_weapons;
};
