#pragma once

#include "Popups/Popup.h"
#include "AppContext.h"

class DeathPopup : public Popup
{
public:
    DeathPopup();

public:
    void update(float dt) override;
    void render() override;

private:
    bool m_isResumePressed = false;
    bool m_isGoToMenuPressed = false;

    CustomButton m_restart;
};