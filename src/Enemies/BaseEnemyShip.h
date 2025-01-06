#pragma once

#include "BaseShip.h"

class ObjectsManager;

class BaseEnemyShip : public BaseShip
{
public:
    BaseEnemyShip(ObjectsManager& om, const VitalityParams& baseVitality, const StageMultipliers& multipliers);

    virtual void initialize() override;

    virtual void update(float dt) override;
    virtual void render() override;
    virtual void onCollision(GameObject* obj) override;

    void setVelocity(const Vector2& velocity);

    float m_thrust = 200000.0f;

protected:
    Texture m_bars;
};
