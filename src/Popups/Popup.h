#pragma once

#include "Popups/Popups.h"

class Popup
{
public:
    Popup();

public:
    virtual void update(float dt);
    virtual void render();

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
private:
    bool m_isVisible = false;
    bool m_inputDisabled = false;

    PopupType m_type = PopupType::Count;
};
