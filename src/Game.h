#pragma once

#include "raylib.h"

#include "AppContext.h"
#include "Background.h"
#include "GravityZone.hpp"
#include "Physics.hpp"
#include "PlayerShip.h"

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

    Physics m_physics;
    GravityZoneSystem m_gravityZones;

    AppContext& m_context;

    PlayerShip m_playerShip;
    bool m_isActive = false;
};
