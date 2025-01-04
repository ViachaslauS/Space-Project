#pragma once

#include "Weapons/BaseWeapon.h"

class ObjectsManager;

class LaserWeapon : public BaseWeapon
{
public:
    LaserWeapon(ObjectsManager& om, int teamId);

public:
    void update(float dt) override;

    virtual void renderCrosshair(Vector2 pos) const override;
    virtual bool hasCustomCrosshair() const {
        return true;
    }

private:
    CrosshairInfo m_crosshair;
};
