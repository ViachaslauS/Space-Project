#include "external/raygui.h"
#include "Popups/Popups.h"
#include "DeathPopup.hpp"

#include "Game.h"

namespace
{
    const std::string DescText = "You are dead!";
    const float descFontSize = 40;
}

DeathPopup::DeathPopup()
    : Popup()
{
    setType(PopupType::LevelUpPopup);

    m_restart.anchor = { 0.0f, 0.0f };

    m_restart.text = "restart";
}

void DeathPopup::update(float dt)
{
    m_restart.rect =
    {
        m_rect.width * 0.5f - m_restart.rect.width * 0.5f, m_rect.height * 0.6f,
        300, 150
    };
    m_restart.origin = { m_rect.x, m_rect.y };
    if (m_restart.isPressed())
    {
        m_game->reset();
        show(false);
    }
}

void DeathPopup::render()
{
    Popup::render();

    const Vector2 descPos = helpers::getDrawPosInRectCenter(m_rect, DescText, descFontSize);
    DrawText(DescText.c_str(), descPos.x, descPos.y - m_rect.height * 0.15f, descFontSize, WHITE);

    m_restart.render({ m_rect.x, m_rect.y });
}
