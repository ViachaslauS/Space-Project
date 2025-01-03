#pragma once

#include <vector>

#include "raymath.h"

#include "Physics.hpp"

struct GravityZone
{
    enum class Direction
    {
        Top,
        Right,
        Down,
        Left,
    };

    Vector2 pos;
    float width;
    float height;
    float remainingTime;

    float phase = 0.0f;

    Direction dir;
    PhysicsComp phys;
};

struct GravityZoneSystem
{
    GravityZoneSystem(Physics &p);

    Physics &physics;


    std::vector<GravityZone> activeZones;

    void addZone(const Vector2 &pos, GravityZone::Direction dir, float activeTime, float width, float height);

    void update(float dt);
    void render();
};
