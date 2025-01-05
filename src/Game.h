#pragma once

#include "raylib.h"

#include "AppContext.h"
#include "Background.h"
#include "GravityZone.hpp"
#include "ObjectsManager.h"
#include "Physics.hpp"
#include "PlayerShip.h"
#include "GameplayManager.h"
#include "HUD/HUDContainer.hpp"
#include "PlayerController.hpp"
#include "Skills.h"

#include <vector>

class Game
{
    friend class Skills;

public:
    Game(AppContext& ctx);

public:
    void update(float dt);
    void render();

    void setActive(bool active);

    const GameplayManager& getGameplayManager() const;
    const PlayerShip& getPlayerShip() const;
    const PlayerController& getPlayerController() const;

    void reset();

    Skills& getSkills();

private:
    Skills m_skills;

    Background m_background;

    ObjectsManager m_objectManager;
    GameplayManager m_gameplayManager;

    Physics m_physics;
    GravityZoneSystem m_gravityZones;

    AppContext& m_context;

    PlayerShip m_playerShip;
    PlayerController m_playerController;

    HUDContainer m_hud;

    bool m_isActive = false;
};
