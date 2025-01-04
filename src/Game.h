#pragma once

#include "raylib.h"

#include "AppContext.h"
#include "Background.h"
#include "GravityZone.hpp"
#include "Physics.hpp"
#include "PlayerShip.h"
#include "GameplayManager.h"
#include "HUD/HUDContainer.hpp"

class Game
{
public:
    Game(AppContext& ctx);

public:
    void update(float dt);
    void render();

    void setActive(bool active);

    const GameplayManager& getGameplayManager() const;

private:
    Background m_background;

    GameplayManager m_gameplayManager;

    Physics m_physics;
    GravityZoneSystem m_gravityZones;

    AppContext& m_context;

    PlayerShip m_playerShip;

    HUDContainer m_hud;

    bool m_isActive = false;
};
