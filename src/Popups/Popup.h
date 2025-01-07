#pragma once

#include "Popups/Popups.h"

#include <raylib.h>
#include <string>

// for button we use hardcoded bg
class CustomButton
{
public:
    CustomButton();

    void init(const Rectangle& inRect, const Vector2& inAnchor, const Vector2& inOrigin, const std::string& inText, int inFont);

    Rectangle rect{};
    Vector2 anchor{};

    Vector2 origin;

    std::string text;
    int font = 32;

    bool isPressed();
    bool isMousePoint();

    void render(Vector2 origin);

    Rectangle getRecTransformed();

private:
    Texture bgTexture;
    Texture bgTextureSelected;
};

class Popup
{
public:
    Popup();

public:
    virtual void update(float dt);
    virtual void render();
    virtual void onStateChanged();

    void setGame(Game* game);

    void show(bool show);

    bool isVisible()
    {
        return m_isVisible;
    }

    void setType(PopupType type)
    {
        m_type = type;
    }
    PopupType getType()
    {
        return m_type;
    }

    Rectangle m_rect{};

protected:
    Game* m_game = nullptr;

    Vector2 m_windowScale{}; // to draw bg

    bool bShowBg = true;
    Color bgColor = { 0, 0, 0, 100 };

private:
    bool m_isVisible = false;
    bool m_inputDisabled = false;

    PopupType m_type = PopupType::Count;

    Texture m_bgTexture;
};
