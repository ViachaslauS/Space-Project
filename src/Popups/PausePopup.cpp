#include "Popups/PausePopup.h"

#include "external/raygui.h"
#include "Popups/Popups.h"

PausePopup::PausePopup(AppContext& ctx)
    : m_context(ctx)
{
    setType(PopupType::PausePopup);

    Vector2 screenSize = { static_cast<float>(GetScreenWidth()),
                           static_cast<float>(GetScreenHeight()) };

    m_rect.width = screenSize.x * 0.3f;
    m_rect.height = screenSize.y * 0.4f;

    m_rect.x = (screenSize.x - m_rect.width) * 0.5f;
    m_rect.y = (screenSize.y - m_rect.height) * 0.55f;

    m_resumeButton.init(
        Rectangle{ 545, 370, 350, 95 },
        { }, {},
        "Resume game", 30
    );
    m_endButton.init(
        Rectangle{ 545, 490, 350, 95 },
        { }, {},
        "End game", 30
    );

}

void PausePopup::update(float dt)
{
    if (m_resumeButton.isPressed())
    {
        m_isResumePressed = false;
        show(false);
        return;
    }
    if (m_endButton.isPressed())
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
    Popup::render();

    m_resumeButton.render({});
    m_endButton.render({});
}