#include "Game.h"

#include "Popups/Popups.h"
#include "Popups/Popup.h"
#include "Popups/PausePopup.h"

Game::Game(AppContext& ctx)
    : m_context(ctx)
{
    m_background = LoadTexture("stars_background.jpg");

    m_playerShip.initialize();
}

void Game::update(float dt)
{
    if (IsKeyPressed(KEY_ESCAPE))
    {
        auto pausePopup = m_context.popups.getPopup(PopupType::PausePopup);
        if (pausePopup != nullptr)
        {
            pausePopup->show(true);
        }
        else
        {
            auto pausePopup = new PausePopup(m_context);
            pausePopup->show(true);
            m_context.popups.addPopup(pausePopup);
        }
    }
    m_playerShip.update(dt);
}

void Game::render()
{
    DrawTexture(m_background, 0, 0, WHITE);

    m_playerShip.render();
}