#include "MainMenuPopup.h"

#include "external/raygui.h"

MainMenuPopup::MainMenuPopup(helpers::MulticastDelegate<>& quitSignal)
    : Popup()
    , m_quitSignal(quitSignal)
{
    setType(PopupType::MainMenuPopup);
}

void MainMenuPopup::update(float dt)
{
    if (m_isStartGamePressed)
    {
        m_isStartGamePressed = false;
        show(false);
        return;
    }
    if (m_isQuit)
    {
        m_quitSignal.broadcast(); 
    }
}

void MainMenuPopup::render()
{
    if (GuiButton((Rectangle { 600, 370, 120, 30 }), "Start Game")) m_isStartGamePressed = true;

    if (GuiButton((Rectangle{ 600, 450, 120, 30 }), "Rage Quit")) m_isQuit = true;

}