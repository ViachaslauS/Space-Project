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
    const Vector2& getPos() const;

    void setRotation(float rot);
    float getRotation() const;

    void applyForce(const Vector2 &dir);

    int getTeamId() const;

    float getCurrentHP() const;

    virtual void onCollision(GameObject *other);
    virtual void onSensorCollision(GameObject *other, bool exit);

    helpers::MulticastDelegate<float, const VitalityData&> OnReceiveDamage;

    ObjectType getObjectType();

    const Vector2& getVelocity() const;

    helpers::MulticastDelegate<> onDieSignal;
    struct VitalityAll
    {
        VitalityParams params;
        VitalityData data;
    };
    VitalityAll getVitality() const { return { m_vitality, m_vitalityData }; }

protected:
    ObjectsManager &m_objectManager;
    VitalityParams m_vitality;
    VitalityData m_vitalityData;
    int m_teamId = -1;

    Texture m_texture;

    // temporary ?
    Vector2 m_pos = { 0.0f, 0.0f };
    Vector2 m_size = { 0.0f, 0.0f };
    float m_rotation = 0.0f;

    float m_xpValue = 0.0f;
};
