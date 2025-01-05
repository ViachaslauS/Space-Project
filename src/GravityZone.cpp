#include <cassert>

#include "raylib.h"
#include "raymath.h"

#include "GravityZone.hpp"

namespace {
    const char* ParticleTexturePath = "gravizone_particle.png";
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
}

GravityZone::GravityZone(ObjectsManager &om,
                         const VitalityParams &vp,
                         const Vector2 &pos,
                         const Vector2 &size,
                         float activeTime,
                         GravityZone::Direction dir)
    : GameObject(om, vp, 0, ObjectType::GravityZone)
    , remainingTime(activeTime)
    , dir(dir)
    , rendering(pos, size, dir)
{
    m_pos = pos;
    m_size = size;
}

GravityZone::~GravityZone()
{
    for (auto a : affectedComps) {
        applyForce(a, true);
    }
}

void GravityZone::render()
{
    rendering.render();
}

void GravityZone::update(float dt)
{
    rendering.update(dt);
}

void GravityZone::applyForce(PhysicsComp *comp, bool exit)
{
    Vector2 vec;
    switch (dir) {
    case Direction::Top:
        vec.y = -force;
        vec.x = 0.0f;
        break;
    case Direction::Right:
        vec.y = 0.0f;
        vec.x = force;
        break;
    case Direction::Down:
        vec.y = force;
        vec.x = 0.0f;
        break;
    case Direction::Left:
        vec.y = 0.0f;
        vec.x = -force;
        break;
    }

    // TODO: damage if there's opposing force
    Vector2 res;
    if (exit) {
        assert(std::find(affectedComps.begin(), affectedComps.end(), comp) != affectedComps.end());
        affectedComps.erase(std::remove(affectedComps.begin(), affectedComps.end(), comp));
        res = Vector2Subtract(comp->gravityZoneForce, vec);
    } else {
        affectedComps.push_back(comp);
        res = Vector2Add(comp->gravityZoneForce, vec);
    }

    comp->gravityZoneForce = res;
}

void GravityZone::onSensorCollision(GameObject *other, bool exit) {
    auto move = false;
    switch (other->m_objectType) {
    case ObjectType::RocketProjectile:
    case ObjectType::Asteroid:
    case ObjectType::EnemyShip:
        applyForce(other->m_physicsComp, exit);
    default:
        break;
    }
}

GravityZoneSystem::GravityZoneSystem(Physics &p, ObjectsManager &om)
    : physics(p)
    , objManager(om)
    {}

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
    m_particleTexture = LoadTexture(ParticleTexturePath);
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
    float rotation = 0.0f;
    switch (m_currDirection)
    {
    case GravityZone::Direction::Top:
        rotation = 270.0f;
        break;
    case GravityZone::Direction::Right:
        rotation = 0.0f;
        break;
    case GravityZone::Direction::Down:
        rotation = 90.0f;
        break;
    case GravityZone::Direction::Left:
        rotation = 180.0f;
        break;
    default: break;
    }

    for (const auto& particle : m_particles)
    {
        Color color = WHITE;

        color.a = helpers::lerpTudaSuda(particle.progress) * 255;

        const Vector2 pos = calculatePos(particle);

        Rectangle source = { 0.0f, 0.0f, (float)m_particleTexture.width, (float)m_particleTexture.height };
        Rectangle dest = { pos.x, pos.y, (float)m_particleTexture.width, (float)m_particleTexture.height };
        Vector2 origin = { m_particleTexture.width * 0.5f, m_particleTexture.height * 0.5f };

        DrawTexturePro(m_particleTexture, source, dest, origin, rotation, color);
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

    particle.spawnDelay = helpers::randFlt() * 2.0f;
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
    DrawTextureNPatch(bg, GravityZoneNPatch, bounds, {}, 0, WHITE);
}

void GravityZone::Rendering::update(float dt) {
    particles.update(dt);
}
