#pragma once

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

private:
    bool m_isVisible = false;
    bool m_inputDisabled = false;
};
