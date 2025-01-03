#include "Popups/PausePopup.h"

#include "external/raygui.h"
#include "Popups/Popups.h"

PausePopup::PausePopup(AppContext& ctx)
    : m_context(ctx)
{
}

void PausePopup::update(float dt)
{
    if (m_isResumePressed)
    {
        m_isResumePressed = false;
        show(false);
        return;
    }
    if (m_isGoToMenuPressed)
    {
        m_isGoToMenuPressed = false;
        show(false);
        auto mainMenuPopup = m_context.popups.getPopup(PopupType::MainMenuPopup);
        if (mainMenuPopup != nullptr)
        {
            mainMenuPopup->show(true);
        }
        return;
    }
}

void PausePopup::render()
{
    if (GuiButton((Rectangle { 600, 370, 120, 30 }), "Resume Game")) 
    {
        m_isResumePressed = true;
    }

    if (GuiButton((Rectangle{ 600, 450, 120, 30 }), "Go to menu")) 
    {
        m_isGoToMenuPressed = true;
    }
}