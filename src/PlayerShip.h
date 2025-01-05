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

    void initialize() override;
    void update(float dt) override;
    void reset() override;
    void render() override;
    void onCollision(GameObject *other) override;

protected:
    GravityZoneSystem &m_gravityZones;
    float m_offsetProgress = 0.0f;
};
