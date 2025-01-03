#pragma once

#include "raylib.h"
#include <PlayerShip.h>

#include "AppContext.h"

class Game
{
public:
    Game(AppContext& ctx);
public:
    void update(float dt);
    void render();

private:
    Texture m_background;
    AppContext& m_context;

    PlayerShip m_playerShip;
};