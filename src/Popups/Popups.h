#pragma once

#include <vector>

class Popup;
class Game;

enum class PopupType
{
    MainMenuPopup,
    DefeatPopup,
    LevelUpPopup,
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

    void setGame(Game* game);

    void update(float dt);
    void render();

    bool isCoveredPopup();

private:
    std::vector<Popup*> m_popups;
    Game* m_game = nullptr;
};
