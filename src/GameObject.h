#pragma once

#include "Vitality.h"
#include "Helpers.h"

#include "raylib.h"

enum class ObjectType
{
    GravityZone,
    PlayerShip,
    EnemyProjectile,
    PlayerProjectile,
    EnemyMissile,
    PlayerMissile,
    Asteroid,
};

// base class for all in-game object like asteroids, enemy ships etc.
class GameObject
{
public:
    // for physics collisions
    ObjectType m_objectType;

    GameObject(const VitalityParams& vitality, int teamId, ObjectType type);
    virtual ~GameObject();

    virtual void initialize();

    virtual void update(float dt);
    virtual void render();

    void damage(float damage);
    bool isDead() const;
    virtual void OnDie();

    virtual void reset();

    Vector2 center() const;

    int getTeamId() const;

    virtual void onCollision(GameObject *other);

    helpers::MulticastDelegate<float, const VitalityData&> OnReceiveDamage;

    ObjectType getObjectType();
protected:
    VitalityParams m_vitality;
    VitalityData m_vitalityData;
    int m_teamId = -1;

    Texture m_texture;

    // temporary ?
    Vector2 m_pos = { 0.0f, 0.0f };
    Vector2 m_size = { 0.0f, 0.0f };
};
