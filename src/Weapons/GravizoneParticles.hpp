#pragma once

#include <raylib.h>
#include <array>

#include "GravityZone.hpp"
#include "Helpers.h"

class GravizoneParticles
{
    static constexpr int MaxParticles = 10;

    struct Particle
    {
        float widthOffset = 0.0f;
        float progress = 0.0f;
        float speed = 0.0f;
        float spawnDelay = 0.0f;
    };

public:
    GravizoneParticles();

    void setBounds(Rectangle rect);

    void update(float dt);
    void render() const;

    void changeDirection(GravityZone::Direction newDirection);

private:
    void reset();
    void resetParticle(Particle& particle);

    Vector2 calculatePos(const Particle& particle) const;

private:
    GravityZone::Direction m_currDirection = GravityZone::Direction::Right;

    std::array<Particle, MaxParticles> m_particles;

    Rectangle m_rect;
    Texture m_particleTexture;
};