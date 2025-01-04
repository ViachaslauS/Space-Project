#pragma once

#include "Weapons/BaseWeapon.h"

class RocketWeapon : public BaseWeapon
{
public:
    RocketWeapon(int teamId);

public:
    void update(float dt) override;
};