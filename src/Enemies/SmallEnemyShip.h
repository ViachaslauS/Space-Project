#pragma once

#include "Enemies/BaseEnemyShip.h"

class SmallEnemyShip : public BaseEnemyShip
{
public:
    SmallEnemyShip();

public:
    void initialize() override;
};