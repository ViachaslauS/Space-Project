#include "MainMenuPopup.h"

#include "external/raygui.h"

MainMenuPopup::MainMenuPopup(helpers::MulticastDelegate<>& quitSignal)
    : Popup()
    , m_quitSignal(quitSignal)
{
    setType(PopupType::MainMenuPopup);

    Vector2 screenSize = { static_cast<float>(GetScreenWidth()),
                          static_cast<float>(GetScreenHeight()) };

    m_rect.width = screenSize.x * 0.3f;
    m_rect.height = screenSize.y * 0.4f;

    m_rect.x = (screenSize.x - m_rect.width) * 0.5f;
    m_rect.y = (screenSize.y - m_rect.height) * 0.55f;

    m_startBtn.init(
        Rectangle{ 545, 370, 350, 95 },
        { }, {},
        "Start game", 30
    );
    m_quitBtn.init(
        Rectangle{ 545, 490, 350, 95 },
        { }, {},
        "Quit game", 30
    );
}

void MainMenuPopup::update(float dt)
{
    if (m_startBtn.isPressed())
    {
        m_isStartGamePressed = false;
        show(false);
        return;
    }
    if (m_quitBtn.isPressed())
    {
        m_quitSignal.broadcast(); 
    }
}

void MainMenuPopup::render()
{
    Popup::render();

    m_startBtn.render({});
    m_quitBtn.render({});
}