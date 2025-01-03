#pragma once

#include <vector>

class Popup;

enum class PopupType
{
    MainMenuPopup,
    DefeatPopup,
    UpgradeSkillPopup,
    PausePopup,

    Count,
};

class Popups
{
public:
    Popups();

public:
    void addPopup(Popup* popup);
    void killPopup(Popup* popup);

    Popup* getPopup(PopupType type);

    void update(float dt);
    void render();

    bool isCoveredPopup();

private:
    std::vector<Popup*> m_popups;
};
