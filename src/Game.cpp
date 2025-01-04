#include "Game.h"

#include "Popups/Popups.h"
#include "Popups/Popup.h"
#include "Popups/PausePopup.h"
#include "Popups/LevelUpPopup.h"
#include "PlayerStats.h"

Game::Game(AppContext& ctx)
    : m_context(ctx)
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
        if (IsKeyPressed(KEY_ESCAPE))
        {
            auto pausePopup = m_context.popups.getPopup(PopupType::PausePopup);
            if (pausePopup == nullptr)
            {
                pausePopup = new PausePopup(m_context);
                m_context.popups.addPopup(pausePopup);
            }
            pausePopup->show(true);
        }
        m_playerShip.update(dt);
    }

    m_background.update(dt);
}

void Game::render()
{
    m_background.render();
    m_playerShip.render();
}

void Game::setActive(bool active)
{
    m_isActive = active;
}