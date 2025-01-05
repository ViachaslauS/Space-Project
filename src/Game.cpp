#include "utils.h"

#include "Game.h"
#include "Helpers.h"
#include "Popups/Popups.h"
#include "Popups/Popup.h"
#include "Popups/PausePopup.h"
#include "Popups/LevelUpPopup.h"
#include "PlayerStats.h"

Game::Game(AppContext& ctx)
    : m_objectManager(m_physics)
    , m_gameplayManager(m_physics, m_objectManager, &m_playerShip)
    , m_gravityZones(m_physics, m_objectManager)
    , m_context(ctx)
    , m_playerShip(m_objectManager, m_gravityZones)
    , m_hud(*this)
{
    m_physics.createRectangularBody(helpers::windowCenter(), 100.0f, 50.0f, &m_playerShip);
    m_playerShip.initialize();
    m_playerController.setShip(&m_playerShip);

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

    m_hud.init({});
}

void Game::update(float dt)
{
    if (m_isActive)
    {
        m_physics.update(); // processing physics must come before all other updates

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
        }
        m_playerShip.update(dt);
        m_playerController.update(dt);
        m_gameplayManager.update(dt);
        m_gravityZones.update(dt);
        m_hud.update(dt);
        m_background.update(dt);
    }
}

void Game::render()
{
    m_background.render();

    m_gameplayManager.render();

    m_playerShip.render();

    m_gravityZones.render();

    if (m_isActive)
    {
        m_playerController.render();
        m_hud.render();
    }

    m_physics.debugRender();
}

void Game::setActive(bool active)
{
    m_isActive = active;

    ShowCursor();
}

const GameplayManager& Game::getGameplayManager() const
{
    return m_gameplayManager;
}

const PlayerShip& Game::getPlayerShip() const
{
    return m_playerShip;
}

const PlayerController& Game::getPlayerController() const
{
    return m_playerController;
}

void Game::reset()
{
    m_skills.resetSkills(*this);
}

Skills& Game::getSkills()
{
    return m_skills;
}
