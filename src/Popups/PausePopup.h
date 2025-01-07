#pragma once

#include "Popups/Popup.h"
#include "AppContext.h"

class PausePopup : public Popup
{
public:
    PausePopup(AppContext& ctx);

public:
    void update(float dt) override;
    void render() override;

private:
    AppContext& m_context;
    
    bool m_isResumePressed = false;
    bool m_isGoToMenuPressed = false;

    CustomButton m_resumeButton;
    CustomButton m_endButton;
};