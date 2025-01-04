#pragma once

#include <memory>

class PlayerShip;

class PlayerController
{
public:
    void update(float dt);
    void render();

    void setShip(PlayerShip* ship);

protected:
    void handleInput();
    void selectActiveWeapon(int idx);

protected:
    PlayerShip* m_ship = nullptr;

    int32_t m_selectedWeaponIdx = 0;
};
