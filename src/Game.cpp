#include "utils.h"

#include "Game.h"

#include "Popups/Popups.h"
#include "Popups/Popup.h"
#include "Popups/PausePopup.h"
#include "Popups/LevelUpPopup.h"
#include "PlayerStats.h"

Game::Game(AppContext& ctx)
    : m_context(ctx)
    , m_gravityZones(m_physics)
{
    m_playerShip.initialize();

    //Init stats and levelup callback
    auto& stats = PlayerStats::get();
    stats.onLevelUp.add([this]() {
        auto popup = m_context.popups.getPopup(PopupType::LevelUpPopup);
        if (popup == nullptr)
        {
            popup = new LevelUpPopup();
            m_context.popups.addPopup(popup);
        }
        popup->show(true);
    });
}

void Game::update(float dt)
{
    if (m_isActive)
    {
        // Here should be logic for unpaused game
        if (IsKeyReleased(KEY_ESCAPE))
        {
            auto pausePopup = m_context.popups.getPopup(PopupType::PausePopup);
            if (pausePopup == nullptr)
            {
                pausePopup = new PausePopup(m_context);
                m_context.popups.addPopup(pausePopup);
            }
            pausePopup->show(true);
        } else if (IsKeyReleased(KEY_BACKSLASH)) {
            m_physics.toggleDebugRender();
        } else if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            TRACELOG(LOG_ERROR, "MOUSE BUTTON RELEASED");
            GravityZone::Direction dir;
            if (IsKeyPressed(KEY_W)) {
                dir = GravityZone::Direction::Top;
            } else if (IsKeyPressed(KEY_D)) {
                dir = GravityZone::Direction::Right;
            } else if (IsKeyPressed(KEY_S)) {
                dir = GravityZone::Direction::Down;
            } else if (IsKeyPressed(KEY_A)) {
                dir = GravityZone::Direction::Left;
            } else {
                goto noZoneDirection;
            }

            auto pos = GetMousePosition();
            m_gravityZones.addZone(pos, dir, 3.0f, 150.0f, 150.0f);
        }
        noZoneDirection:
        m_playerShip.update(dt);
        m_gameplayManager.update(dt);
        m_gravityZones.update(dt);
        m_physics.update();
    }

    m_background.update(dt);
}

void Game::render()
{
    m_background.render();

    m_gameplayManager.render();

    m_playerShip.render();

    m_gravityZones.render();

    m_physics.debugRender();
}

void Game::setActive(bool active)
{
    m_isActive = active;
}
