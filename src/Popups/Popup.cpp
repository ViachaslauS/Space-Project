#include "Popup.h"

#include <raymath.h>

#include "Helpers.h"

namespace
{
    constexpr Rectangle bgRectPopup
    {
        0, 0,
        458.0f, 166.0f
    };

    const char* bgPathButton = "popup/window.png";
    const char* bgPathButtonSelected = "popup/window_selected.png";

    constexpr Rectangle bgRectButton
    {
        0, 0,
        458.0f, 166.0f
    };

    const NPatchInfo bgNpatchButton
    {
        .source = bgRectPopup,
        .left = 37,
        .top = 34,
        .right = 40,
        .bottom = 36,
        .layout = NPATCH_NINE_PATCH
    };

    const Rectangle bgRectButtonSelected
    {
        0, 0,
        387.0f, 102.0f
    };

    const NPatchInfo bgNpatchButtonSelected
    {
        .source = bgRectButtonSelected,
        .left = 41,
        .top = 35,
        .right = 41,
        .bottom = 35,
        .layout = NPATCH_NINE_PATCH
    };

    const char* bgPathPopup = "popup/window.png";

    const NPatchInfo bgNpatchPopup
    {
        .source = bgRectPopup,
        .left = 37,
        .top = 34,
        .right = 40,
        .bottom = 36,
        .layout = NPATCH_NINE_PATCH
    };
}

Popup::Popup()
{
    Vector2 screenSize = { GetScreenWidth(), GetScreenHeight() };

    m_rect.width = screenSize.x * 0.6f;
    m_rect.height = screenSize.y * 0.6f;

    m_rect.x = (screenSize.x - m_rect.width) * 0.5f;
    m_rect.y = (screenSize.y - m_rect.height) * 0.5f;

    m_bgTexture = LoadTexture(bgPathPopup);
}

void Popup::show(bool show)
{
    m_isVisible = show;
    onStateChanged();
}

void Popup::update(float dt)
{

}

void Popup::render()
{
    DrawTextureNPatch(m_bgTexture, bgNpatchPopup, m_rect, {}, 0.0f, WHITE);
}

void Popup::onStateChanged()
{

}

void Popup::setGame(Game* game)
{
    m_game = game;
}

CustomButton::CustomButton()
{
    text = "";
    bgTexture = LoadTexture(bgPathButton);
    bgTextureSelected = LoadTexture(bgPathButtonSelected);
}

bool CustomButton::isPressed()
{
    if (isMousePoint())
    {
        return IsMouseButtonReleased(MOUSE_BUTTON_LEFT);
    }

    return false;
}

bool CustomButton::isMousePoint()
{
    return CheckCollisionPointRec(GetMousePosition(), getRecTransformed());
}

Rectangle CustomButton::getRecTransformed()
{
    Rectangle outRec = rect;
    outRec.x += origin.x - anchor.x * rect.width;
    outRec.y += origin.y - anchor.y * rect.height;

    return outRec;
}

void CustomButton::render(Vector2 origin)
{
    const auto transformed = getRecTransformed();
    Rectangle rectDraw = rect;
    rectDraw.x += origin.x;
    rectDraw.y += origin.y;

    DrawTextureNPatch(bgTexture, bgNpatchButton, 
        rectDraw, { anchor.x * rectDraw.width, anchor.y * rectDraw.height }, 0.0f, WHITE);

    const Vector2 textPos = helpers::getDrawPosInRectCenter(rectDraw, text, font);

    DrawText(text.c_str(), textPos.x, textPos.y, font, WHITE);

    if (isMousePoint())
    {
        Rectangle selectedRect
        {
            rectDraw.x + bgNpatchButton.left,
            rectDraw.y + bgNpatchButton.top,

            rectDraw.width - (bgNpatchButton.left + bgNpatchButton.right),
            rectDraw.height - (bgNpatchButton.top + bgNpatchButton.bottom),

        };

        DrawTextureNPatch(bgTextureSelected, bgNpatchButtonSelected, 
            selectedRect, { anchor.x * selectedRect.width, anchor.y * selectedRect.height }, 0.0f, WHITE);
    }
}
