#pragma once

#include "raylib.h"
#include "raymath.h"
#include "Helpers.h"

#include "GameObject.h"

class Projectile : public GameObject
{
public:
    Texture texture;

    float damage = 0.0f;

    enum class State
    {
        Alive,
        Unused,
    };

    State state = State::Unused;

    Projectile(ObjectsManager &om, int teamId, ObjectType type);
    Projectile(const Projectile& proj);

    State getState();
    void setState(State newState);

    void update(float dt) override;
    void render() override;
    void setVelocity(const Vector2& velocity);
    void onCollision(GameObject* obj) override;
};
