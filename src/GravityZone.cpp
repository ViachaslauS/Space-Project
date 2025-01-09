#include <cassert>

#include "raylib.h"
#include "raymath.h"

#include "GravityZone.hpp"

namespace {
    const char* GravizoneBGNPPath = "gravizone_bg_np.png";
    const NPatchInfo GravityZoneNPatch =
    {
        .source{ 0, 0, 5, 1 },
        .left{0},
        .top{0},
        .right{0},
        .bottom{0},
        .layout{NPATCH_NINE_PATCH}
    };

    void processZoneCollision(Physics& phys, GravityZone& z, GameObject *o)
    {
        Vector2 vec;
        switch (z.dir) {
        case GravityZone::Direction::Top:
            vec.y = -z.force;
            vec.x = 0.0f;
            break;
        case GravityZone::Direction::Right:
            vec.y = 0.0f;
            vec.x = z.force;
            break;
        case GravityZone::Direction::Down:
            vec.y = z.force;
            vec.x = 0.0f;
            break;
        case GravityZone::Direction::Left:
            vec.y = 0.0f;
            vec.x = -z.force;
            break;
        }

        phys.applyForce(o->m_physicsComp, vec);
        o->damage(z.damage * 0.016f);
    }
}

GravityZone::GravityZone(ObjectsManager &om,
                         const VitalityParams &vp,
                         const Vector2 &pos,
                         const Vector2 &size,
                         float activeTime,
                         GravityZone::Direction dir,
                         float damage)
    : GameObject(om, vp, 0, ObjectType::GravityZone)
    , remainingTime(activeTime)
    , dir(dir)
    , rendering(pos, size, dir)
    , damage(damage)
{
    m_pos = pos;
    m_size = size;
    bounds.x = pos.x;
    bounds.y = pos.y;
    bounds.width = size.x;
    bounds.height = size.y;
}

void GravityZone::render()
{
    rendering.render();
}

void GravityZone::update(float dt)
{
    rendering.update(dt);
}

GravityZoneSystem::GravityZoneSystem(Physics &p, ObjectsManager &om)
    : physics(p)
    , objManager(om)
{
    physics.gravityZones = this;
}

void GravityZoneSystem::addZone(const Vector2 &pos, GravityZone::Direction dir, float damage)
{
    VitalityParams vp;

    Vector2 size;
    if (dir == GravityZone::Direction::Left || dir == GravityZone::Direction::Right)
    {
        size = { params.height, params.width };
    }
    else
    {
        size = { params.width, params.height };
    }

    auto &newZone = activeZones.emplace_back(std::make_unique<GravityZone>(
                                                 objManager,
                                                 vp,
                                                 pos,
                                                 size,
                                                 params.gzLifetime,
                                                 dir,
                                                 damage));
}

void GravityZoneSystem::update(float dt)
{
    for (auto i = 0; i < activeZones.size();)
    {
        const auto &z = activeZones[i];
        z->update(dt);

        physics.checkRectangleCollision(activeZones[i]->bounds, [this, &z](GameObject *o) {
            processZoneCollision(physics, *z, o);
        });

        z->remainingTime -= dt;
        if (z->remainingTime < 0.0f)
        {
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

GravityZone::Particles::Particles()
{
    reset();
}

void GravityZone::Particles::setBounds(Rectangle rect)
{
    m_rect = rect;
}

void GravityZone::Particles::update(float dt)
{
    for (auto& particle : m_particles)
    {
        if (particle.spawnDelay > 0.0f)
        {
            particle.spawnDelay -= dt;
            continue;
        }

        particle.progress += particle.speed * dt;

        if (particle.progress >= 1.0f)
        {
            resetParticle(particle);
        }
    }
}

void GravityZone::Particles::render() const
{
    Vector2 size;

    constexpr float width = 3;
    constexpr float height = 10;

    switch (m_currDirection)
    {
    case GravityZone::Direction::Top:
    case GravityZone::Direction::Down:
        size.x = width;
        size.y = height;
        break;
    case GravityZone::Direction::Right:
    case GravityZone::Direction::Left:
        size.x = height;
        size.y = width;
        break;
    default: break;
    }

    for (const auto& particle : m_particles)
    {
        Color color = BLUE;

        color.a = helpers::lerpTudaSuda(particle.progress) * 255;

        const Vector2 pos = calculatePos(particle);

        DrawRectangleV(pos, size, color);
    }
}

void GravityZone::Particles::setDirection(GravityZone::Direction newDirection)
{
    if (m_currDirection != newDirection)
    {
        m_currDirection = newDirection;
        reset();
    }
}

void GravityZone::Particles::reset()
{
    for (auto& particle : m_particles)
    {
        resetParticle(particle);
    }
}

void GravityZone::Particles::resetParticle(Particle& particle)
{
    particle.progress = 0.0f;
    particle.widthOffset = std::lerp(0.1f, 0.9f, helpers::randFlt());

    particle.spawnDelay = helpers::randFlt() * 0.3f;
    particle.speed = 0.1f + helpers::randFlt() * 4.0f;
}

Vector2 GravityZone::Particles::calculatePos(const Particle& particle) const
{
    Vector2 pos{};
    switch (m_currDirection)
    {
    case GravityZone::Direction::Top:
        pos.x = m_rect.x + m_rect.width * particle.widthOffset;
        pos.y = m_rect.y + m_rect.height * (1.0f - particle.progress);
        break;
    case GravityZone::Direction::Right:
        pos.x = m_rect.x + m_rect.width * particle.progress;
        pos.y = m_rect.y + m_rect.height * particle.widthOffset;
        break;
    case GravityZone::Direction::Down:
        pos.x = m_rect.x + m_rect.width * particle.widthOffset;
        pos.y = m_rect.y + (m_rect.height * particle.progress);
        break;
    case GravityZone::Direction::Left:
        pos.x = m_rect.x + m_rect.width * (1.0f - particle.progress);
        pos.y = m_rect.y + m_rect.height * particle.widthOffset;
        break;
    default:
        break;
    }

    return pos;
}

GravityZone::Rendering::Rendering(const Vector2 &pos, const Vector2 &size, GravityZone::Direction dir)
{
    bg = LoadTexture(GravizoneBGNPPath);

    bounds.x = pos.x - size.x * 0.5f;
    bounds.y = pos.y - size.y * 0.5f;

    bounds.width = size.x;
    bounds.height = size.y;

    particles.setDirection(dir);
    particles.setBounds(bounds);
}

void GravityZone::Rendering::render() const {
    particles.render();
}

void GravityZone::Rendering::update(float dt) {
    particles.update(dt);
}
