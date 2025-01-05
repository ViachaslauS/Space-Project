#pragma once

#include "raylib.h"
#include "raymath.h"
#include "Helpers.h"

#include "GameObject.h"

namespace
{
constexpr VitalityParams ProjectileBaseVitality =
    {
        false,
        1.0f,
        false,
        ShieldParams
        {
            0.0f,
            0.0f,
            0.0f
        }
    };
    constexpr float AsteroidDamage = 20.0f;
}

struct Projectile : public GameObject
{
    Texture texture;
    Vector2 pos = Vector2{ 0, 0 };
    float damage = 0.0f;

    enum class State
    {
        Alive,
        Unused,
    };

    State state = State::Unused;

    Projectile(ObjectsManager &om, int teamId, ObjectType type)
        :GameObject(om, ProjectileBaseVitality, teamId, type)
    {
        
    }

    Projectile(const Projectile& proj)
        :GameObject(proj.m_objectManager, ProjectileBaseVitality, proj.m_teamId, proj.m_objectType)
    {
        texture = proj.texture;
        damage = proj.damage;
    }
    State getState()
    {
        return state;
    }
    void setState(State newState)
    {
        state = newState;
    }

    void update(float dt) override
    {
        if (state == State::Alive)
        {
            if (helpers::isInWindow(m_pos) == false)
            {
                OnDie();
            }
        }
    }

    void render() override
    {
        if (state == State::Alive)
        {
            auto drawPos = m_pos;
            drawPos.y -= texture.height * 0.5f;
            drawPos.x -= texture.width * 0.5f;
            DrawTextureEx(texture, drawPos, m_rotation * (180/PI), 1.0f, WHITE);
        }
    }

    void setVelocity(const Vector2& velocity)
    {
        m_physicsComp->physics->setVelocityWithRotation(m_physicsComp, velocity);
    }

    void onCollision(GameObject* obj) override
    {
        if (obj->getTeamId() != m_teamId)
        {
            obj->damage(damage);
            OnDie();
        }
    }
};