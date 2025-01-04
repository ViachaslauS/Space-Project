#include "raylib.h"

#include "GravityZone.hpp"

GravityZoneSystem::GravityZoneSystem(Physics &p)
    : physics(p) {}

void GravityZoneSystem::addZone(const Vector2 &pos, GravityZone::Direction dir, float activeTime, float width, float height)
{
    auto physComp = physics.createRectangularBody(pos, width, height);
    activeZones.push_back({
            pos,
            width,
            height,
            activeTime,
            0.0f,
            dir,
            physComp,
        });
}

void GravityZoneSystem::update(float dt)
{
    for (auto i = 0; i < activeZones.size();)
    {
        activeZones[i].phase += dt;
        activeZones[i].remainingTime -= dt;
        if (activeZones[i].remainingTime < 0.0f)
        {
            physics.removeBody(activeZones[i].phys);
            activeZones[i] = activeZones.back();
            activeZones.pop_back();
        } else {
            i++;
        }
    }
}

void GravityZoneSystem::render()
{
    // TODO: add gradient render depending on direction
    for (const auto &z : activeZones)
    {
        Rectangle rec;

        rec.x = z.pos.x - z.width / 2.0f;
        rec.y = z.pos.y - z.height / 2.0f;
        rec.width = z.width;
        rec.height = z.height;

        Color c { 0x00, 0xff, 0x00, 0x30 };
        DrawRectangleRec(rec, c);
    }
}
