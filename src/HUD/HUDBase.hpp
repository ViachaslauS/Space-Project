#pragma once

#include "raylib.h"

class Game;

class HUDBase
{
public:
    HUDBase(Game& game);
    virtual ~HUDBase() = default;

    virtual void init(Vector2 relPos);

    virtual void update(float dt);
    virtual void render();

    virtual void reset();

protected:
    Vector2 getConvertedPos() const;

protected:
    Game& m_game;
    Vector2 m_relPos;
};