#include "Game.h"

#include "Popups/Popups.h"
#include "Popups/Popup.h"
#include "Popups/PausePopup.h"

Game::Game(AppContext& ctx)
    : m_context(ctx)
{
    m_playerShip.initialize();
}

void Game::update(float dt)
{
    if (m_isActive)
    {
        // Here should be logic for unpaused game
        if (IsKeyPressed(KEY_ESCAPE))
        {
            auto pausePopup = m_context.popups.getPopup(PopupType::PausePopup);
            if (pausePopup != nullptr)
            {
                pausePopup->show(true);
            }
            else
            {
                pausePopup = new PausePopup(m_context);
                pausePopup->show(true);
                m_context.popups.addPopup(pausePopup);
            }
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