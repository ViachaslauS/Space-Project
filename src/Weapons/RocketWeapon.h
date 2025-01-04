#pragma once

#include "Weapons/BaseWeapon.h"

class ObjectsManager;

class RocketWeapon : public BaseWeapon
{
public:
    RocketWeapon(ObjectsManager &om, int teamId);

public:
    void update(float dt) override;
};
