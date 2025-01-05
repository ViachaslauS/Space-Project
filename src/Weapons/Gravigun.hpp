#pragma once

#include "Weapons/BaseWeapon.h"
#include "Weapons/GravizoneParticles.hpp"

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

protected:
    GravityZoneSystem &m_gravityZones;
    Texture m_gravizoneBG;
    Params m_params;

    Rectangle m_bounds{ 0.0f, 0.0f, 0.0f, 0.0f };

    GravizoneParticles m_particles;
    GravityZone::Direction m_currDirection = GravityZone::Direction::Right;
};
