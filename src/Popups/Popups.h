#pragma once

#include <vector>

class Popup;

class Popups
{
public:
    Popups();

public:
    void addPopup(Popup* popup);
    void killPopup(Popup* popup);

    void update(float dt);
    void render();

private:
    std::vector<Popup*> m_popups;
};
