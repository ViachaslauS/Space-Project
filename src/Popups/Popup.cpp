#include "Popup.h"

#include <raymath.h>

#include "Helpers.h"

namespace
{
    constexpr Rectangle bgRectPopup
    {
        0, 0,
        433.0f, 148.0f
    };

    const char* bgPathButton = "popup/button.png";
    const char* bgPathButtonSelected = "popup/window_selected.png";

    constexpr Rectangle bgRectButton
    {
        0, 0,
        433.0f, 148.0f
    };

    const NPatchInfo bgNpatchButton
    {
        .source = bgRectPopup,
        .left = 14,
        .top = 14,
        .right = 14,
        .bottom = 14,
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

    const char* bgPathPopup = "popup/popup.png";

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
    Vector2 screenSize = { static_cast<float>(GetScreenWidth()),
                           static_cast<float>(GetScreenHeight()) };

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
    if (bShowBg)
    {
        DrawRectangle(0.0f, 0.0f, GetScreenWidth(), GetScreenHeight(), bgColor);
    }
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

void CustomButton::init(const Rectangle& inRect, const Vector2& inAnchor, const Vector2& inOrigin, const std::string& inText, int inFont)
{
    rect = inRect;
    anchor = inAnchor;
    origin = inOrigin;
    text = inText;
    font = inFont;
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
