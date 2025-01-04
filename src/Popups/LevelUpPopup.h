#pragma once 

#include "Popups/Popup.h"
#include "Skills.h"

class LevelUpPopup : public Popup
{
public:
    LevelUpPopup();

public:
    void update(float dt) override;
    void render() override;
    void onStateChanged() override;

private:
    void calculateSkills();

private:
    std::vector<Skills::Skills> m_upgradeSkills;

    bool m_noUpgradeSkills = false;
    int m_skillUpgradeNum = -1;
};