#pragma once

#include "GameObject.h"
#include "BaseShip.h"

class ObjectsManager;
class Skills;
struct GravityZoneSystem;

class PlayerShip : public BaseShip
{
    friend class Skills;
public:
    PlayerShip(ObjectsManager &om, GravityZoneSystem &gz);

    virtual void initialize() override;
    virtual void update(float dt) override;
    virtual void reset() override;
    virtual void render() override;

protected:
    GravityZoneSystem &m_gravityZones;
    float m_offsetProgress = 0.0f;
};
