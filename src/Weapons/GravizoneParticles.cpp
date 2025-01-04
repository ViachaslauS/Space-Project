#include "Weapons/GravizoneParticles.hpp"

namespace
{
    const char* ParticleTexturePath = "gravizone_particle.png";
}

GravizoneParticles::GravizoneParticles()
{
    m_particleTexture = LoadTexture(ParticleTexturePath);
    reset();
}

void GravizoneParticles::setBounds(Rectangle rect)
{
    m_rect = rect;
}

void GravizoneParticles::update(float dt)
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

void GravizoneParticles::render() const
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

void GravizoneParticles::changeDirection(GravityZone::Direction newDirection)
{
    if (m_currDirection != newDirection)
    {
        m_currDirection = newDirection;
        reset(); 
    }
}

void GravizoneParticles::reset()
{
    for (auto& particle : m_particles)
    {
        resetParticle(particle);
    }
}

void GravizoneParticles::resetParticle(Particle& particle)
{
    particle.progress = 0.0f;
    particle.widthOffset = std::lerp(0.1f, 0.9f, helpers::randFlt());

    particle.spawnDelay = helpers::randFlt() * 2.0f;
    particle.speed = 0.1f + helpers::randFlt() * 4.0f;
}

Vector2 GravizoneParticles::calculatePos(const Particle& particle) const
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
