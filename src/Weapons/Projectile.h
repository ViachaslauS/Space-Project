#pragma once

#include "raylib.h"
#include "raymath.h"
#include "Helpers.h"

struct Projectile
{
    Texture texture;
    Vector2 pos;
    Vector2 speed;
    float damage;
    int teamId;

    enum class State
    {
        Alive,
        Unused,
    };

    State state;
    State getState()
    {
        return state;
    }
    void setState(State newState)
    {
        state = newState;
    }

    void update(float dt)
    {
        if (state == State::Alive)
        {
            pos += Vector2Scale(speed, dt);
            if (helpers::isInWindow(pos) == false)
            {
                state = State::Unused;
            }
        }
    }

    void render()
    {
        if (state == State::Alive)
        {
            auto drawPos = pos;
            drawPos.y -= texture.height * 0.5f;
            DrawTextureV(texture, drawPos, WHITE);
        }
    }
};