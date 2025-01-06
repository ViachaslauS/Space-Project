#pragma once

#include <array>
#include <vector>

#include "GameObject.h"
#include "Physics.hpp"

class ObjectsManager;

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

    GravityZone(ObjectsManager &om,
                const VitalityParams &vp,
                const Vector2 &pos,
                const Vector2 &size,
                float activeTime,
                Direction dir,
                float damage);

    ~GravityZone() override;

    void applyForce(PhysicsComp *comp, bool exit);

    float force = 300000.0f;
    float damage;
    float remainingTime;

    void render() override;
    void update(float dt) override;

    void onSensorCollision(GameObject *other, bool exit) override;

    std::vector<PhysicsComp *> affectedComps;

    class Particles
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
        Particles();

        void setBounds(Rectangle rect);
        void setDirection(GravityZone::Direction newDirection);

        void update(float dt);
        void render() const;

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

    struct Rendering {
        Rendering(const Vector2 &pos, const Vector2 &size, GravityZone::Direction dir);

        Particles particles;
        Texture bg;
        Rectangle bounds{ 0.0f, 0.0f, 0.0f, 0.0f };

        void render() const;
        void update(float dt);
    };

    Rendering rendering;

    Direction dir;
};

struct GravityZoneSystem
{
    GravityZoneSystem(Physics &p, ObjectsManager &om);

    Physics &physics;
    ObjectsManager &objManager;

    std::vector<std::unique_ptr<GravityZone>> activeZones;

    struct GZParams
    {
        float gzLifetime = 1.0f;
        float width = 80.0f;
        float height = 100.0f;
    };

    GZParams params;

    void addZone(const Vector2 &pos, GravityZone::Direction dir, float damage);
    void removeFromAffectedComps(PhysicsComp *comp);

    void update(float dt);
    void render();
};
