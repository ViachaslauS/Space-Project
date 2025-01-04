#include "raylib.h"

#include "GravityZone.hpp"

GravityZone::GravityZone(const VitalityParams &vp,
                         const Vector2 &pos,
                         const Vector2 &size,
                         float activeTime,
                         GravityZone::Direction dir,
                         const PhysicsComp &physComp)
    : GameObject(vp)
    , remainingTime(activeTime)
    , phase(0.0f)
    , dir(dir)
    , phys(phys)
{
    m_pos = pos;
    m_size = size;
}

void GravityZone::render()
{
    Rectangle rec;

    rec.x = m_pos.x - m_size.x / 2.0f;
    rec.y = m_pos.y - m_size.y / 2.0f;
    rec.width = m_size.x;
    rec.height = m_size.y;

    Color c { 0x00, 0xff, 0x00, 0x30 };
    DrawRectangleRec(rec, c);
}

void GravityZone::update(float dt)
{
    phase += dt;
}

GravityZoneSystem::GravityZoneSystem(Physics &p)
    : physics(p) {}

void GravityZoneSystem::addZone(const Vector2 &pos, GravityZone::Direction dir, float activeTime, float width, float height)
{
    VitalityParams p;
    auto physComp = physics.createRectangularBody(pos, width, height, ObjectType::GravityZone);
    Vector2 size { width, height };
    activeZones.emplace_back(
        p,
        pos,
        size,
        activeTime,
        dir,
        physComp);
}

void GravityZoneSystem::update(float dt)
{
    for (auto i = 0; i < activeZones.size();)
    {
        activeZones[i].update(dt);

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
    for (auto &z : activeZones)
    {
        z.render();
    }
}
