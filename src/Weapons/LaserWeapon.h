#pragma once

#include "Weapons/BaseWeapon.h"

class LaserWeapon : public BaseWeapon
{
public:
    LaserWeapon(int teamId);

public:
    void update(float dt) override;

private:
};