#include "raylib.h"

#include "GravityZone.hpp"

GravityZone::GravityZone(ObjectsManager &om,
                         const VitalityParams &vp,
                         const Vector2 &pos,
                         const Vector2 &size,
                         float activeTime,
                         GravityZone::Direction dir)
    : GameObject(om, vp, -1, ObjectType::GravityZone)
    , remainingTime(activeTime)
    , phase(0.0f)
    , dir(dir)
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

void GravityZone::onCollision(GameObject *other) {
    auto move = false;
    switch (other->m_objectType) {
    case ObjectType::RocketProjectile:
    case ObjectType::Asteroid:
        move = true;
    }

    if (move) {

    }
}

GravityZoneSystem::GravityZoneSystem(Physics &p, ObjectsManager &om)
    : physics(p)
    , objManager(om) {}

void GravityZoneSystem::addZone(const Vector2 &pos, GravityZone::Direction dir, float activeTime, float width, float height)
{
    VitalityParams vp;
    Vector2 size { width, height };

    auto &newZone = activeZones.emplace_back(std::make_unique<GravityZone>(
                                                 objManager,
                                                 vp,
                                                 pos,
                                                 size,
                                                 activeTime,
                                                 dir));

    physics.createRectangularBody(pos, width, height, newZone.get());
}

void GravityZoneSystem::update(float dt)
{
    for (auto i = 0; i < activeZones.size();)
    {
        activeZones[i]->update(dt);

        activeZones[i]->remainingTime -= dt;
        if (activeZones[i]->remainingTime < 0.0f)
        {
            physics.removeBody(activeZones[i]->m_physicsComp);
            activeZones[i] = std::move(activeZones.back());
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
        z->render();
    }
}
