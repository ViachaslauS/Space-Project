#pragma once

#include "GameObject.h"
#include "Weapons/BaseWeapon.h"

class ObjectsManager;

enum class WeaponType
{
    LaserWeapon,
    RocketWeapon,

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

    void setPosition(Vector2 pos)
    {
        m_pos = pos;
    }

protected:
    std::vector<BaseWeapon*> m_weapons;
};
