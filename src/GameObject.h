#pragma once

#include "Physics.hpp"
#include "Vitality.h"
#include "Helpers.h"

class ObjectsManager;

enum class ObjectType
{
    GravityZone,
    PlayerShip,
    RocketProjectile,
    LaserProjectile,
    Asteroid,
    EnemyShip,
    Weapon
};

// base class for all in-game object like asteroids, enemy ships etc.
class GameObject
{
public:
    // for physics collisions
    PhysicsComp *m_physicsComp = nullptr;
    const ObjectType m_objectType;

    GameObject(ObjectsManager &objectManger, const VitalityParams& vitality, int teamId, ObjectType objectType);

    void setPhysicsComp(PhysicsComp *physicsComp);

    virtual ~GameObject();

    virtual void initialize();

    virtual void update(float dt);
    virtual void render();

    void damage(float damage);
    bool isDead() const;
    virtual void OnDie();

    virtual void reset();

    Vector2 center() const;

    void setPos(const Vector2 &pos);
    void applyForce(const Vector2 &dir);

    int getTeamId() const;

    virtual void onCollision(GameObject *other);

    helpers::MulticastDelegate<float, const VitalityData&> OnReceiveDamage;

    ObjectType getObjectType();

protected:
    ObjectsManager &m_objectManager;
    VitalityParams m_vitality;
    VitalityData m_vitalityData;
    int m_teamId = -1;

    Texture m_texture;

    // temporary ?
    Vector2 m_pos = { 0.0f, 0.0f };
    Vector2 m_size = { 0.0f, 0.0f };
};
