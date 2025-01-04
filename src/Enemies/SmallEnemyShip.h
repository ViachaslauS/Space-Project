#pragma once

#include "Enemies/BaseEnemyShip.h"

class SmallEnemyShip : public BaseEnemyShip
{
public:
    SmallEnemyShip(ObjectsManager &om);

public:
    void initialize() override;

    ObjectsManager &m_om;
};
