#pragma once

#include "raylib.h"
#include "PlayerShip.h"
#include "Background.h"

#include "AppContext.h"

class Game
{
public:
    Game(AppContext& ctx);
public:
    void update(float dt);
    void render();

    void setActive(bool active);

private:
    Background m_background;
    AppContext& m_context;

    PlayerShip m_playerShip;
    bool m_isActive = false;
};