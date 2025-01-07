#pragma once

#include "Popup.h"
#include "Helpers.h"

class MainMenuPopup : public Popup
{
public:
    MainMenuPopup(helpers::MulticastDelegate<>& quitSignal);

public:
    void update(float dt) override;
    void render() override;

private:
    bool m_isStartGamePressed = false;
    bool m_isQuit = false;

    helpers::MulticastDelegate<>& m_quitSignal;

    CustomButton m_startBtn;
    CustomButton m_quitBtn;
};
