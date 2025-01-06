#pragma once

#include "Weapons/BaseWeapon.h"
#include "GravityZone.hpp"

struct GravityZoneSystem;

class Gravigun : public BaseWeapon
{
public:
    Gravigun(ObjectsManager& om, int teamId, GravityZoneSystem &gz);

public:
    struct Params
    {
        Vector2 size = { 80.0f, 120.0f };
        float lifetime = 0.0f;
        float power = 1.0f;
    };

    void update(float dt) override;

    virtual void renderCrosshair(Vector2 pos) const override;
    virtual bool hasCustomCrosshair() const {
        return true;
    }

    void setParams(Params params);
    Params getParams() const;

protected:
    void handleInput();
    void setDirection(GravityZone::Direction newDirection);

    virtual void shoot() override;

protected:
    GravityZone::Rendering m_crosshair;
    GravityZoneSystem &m_gravityZones;
    Texture m_gravizoneBG;
    Params m_params;

    Texture m_crosshairGravi;
    Texture m_crosshairGraviBack;

    Rectangle m_bounds{ 0.0f, 0.0f, 0.0f, 0.0f };

    GravityZone::Direction m_currDirection = GravityZone::Direction::Right;
};
