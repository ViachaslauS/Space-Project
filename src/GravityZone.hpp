#pragma once

#include <vector>

#include "GameObject.h"
#include "Physics.hpp"

class GravityZone : public GameObject
{
public:
    enum class Direction
    {
        Top,
        Right,
        Down,
        Left,
    };

    GravityZone(const VitalityParams &vp,
                const Vector2 &pos,
                const Vector2 &size,
                float activeTime,
                Direction dir);

    float remainingTime;
    float phase = 0.0f; // for rendering, maybe unneeded

    void render() override;
    void update(float dt) override;

    void onCollision(GameObject *other) override;

    Direction dir;
};

struct GravityZoneSystem
{
    GravityZoneSystem(Physics &p);

    Physics &physics;

    std::vector<std::unique_ptr<GravityZone>> activeZones;

    void addZone(const Vector2 &pos, GravityZone::Direction dir, float activeTime, float width, float height);

    void update(float dt);
    void render();
};
