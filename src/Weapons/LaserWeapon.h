#pragma once

#include "Weapons/BaseWeapon.h"

class ObjectsManager;

class LaserWeapon : public BaseWeapon
{
public:
    LaserWeapon(ObjectsManager &om, int teamId);

public:
    void update(float dt) override;

private:
};
