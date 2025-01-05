#pragma once

#include "Popups/Popups.h"

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

protected:
    Game* m_game = nullptr;

private:
    bool m_isVisible = false;
    bool m_inputDisabled = false;

    PopupType m_type = PopupType::Count;

};
