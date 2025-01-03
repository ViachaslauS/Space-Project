#include "Game.h"

#include "Popups/Popups.h"
#include "Popups/Popup.h"

Game::Game(AppContext& ctx)
    : m_context(ctx)
{
    m_background = LoadTexture("stars_background.jpg");
}

void Game::update(float dt)
{
    if (IsKeyPressed(KEY_ESCAPE))
    {
        auto mainMenuPopup = m_context.popups.getPopup(PopupType::MainMenuPopup);
        if (mainMenuPopup != nullptr)
        {
            mainMenuPopup->show(true);
        }
    }
}

void Game::render()
{
    DrawTexture(m_background, 0, 0, WHITE);
}