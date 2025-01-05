#include "PlayerController.hpp"

#include "PlayerShip.h"
#include "PlayerStats.h"

#include "external/raygui.h"

void PlayerController::update(float dt)
{
    if (!m_ship)
    {
        return;
    }

    handleInput();

    const auto& weapons = m_ship->getWeapons();
    if (m_selectedWeaponIdx < weapons.size())
    {
        bool isCursorShouldBeHidden = weapons[m_selectedWeaponIdx]->hasCustomCrosshair();
        if (isCursorShouldBeHidden != IsCursorHidden())
        {
            isCursorShouldBeHidden ? HideCursor() : ShowCursor();
        }
    }
}

void PlayerController::render()
{
    const auto& weapons = m_ship->getWeapons();
    if (m_selectedWeaponIdx < weapons.size())
    {
        weapons[m_selectedWeaponIdx]->renderCrosshair(GetMousePosition());
    }
}

void PlayerController::setShip(PlayerShip* ship)
{
    m_ship = ship;
    selectActiveWeapon(0);
}

void PlayerController::handleInput()
{
    if (IsKeyPressed(KEY_ONE))
    {
        selectActiveWeapon(0);
    }
    else if (IsKeyPressed(KEY_TWO))
    {
        selectActiveWeapon(1);
    }
    else if (IsKeyPressed(KEY_THREE))
    {
        selectActiveWeapon(2);
    }
    else if (IsKeyPressed(KEY_FOUR))
    {
        selectActiveWeapon(3);
    }
    else if (IsKeyPressed(KEY_FIVE))
    {
        selectActiveWeapon(4);
    }
}

void PlayerController::selectActiveWeapon(int idx)
{
    if (!m_ship)
    {
        return;
    }

    const auto& weapons = m_ship->getWeapons();
    if (helpers::isValidIdx(weapons, idx))
    {
        if (helpers::isValidIdx(weapons, m_selectedWeaponIdx))
        {
            weapons[m_selectedWeaponIdx]->setActive(false);
        }

        m_selectedWeaponIdx = idx;
        weapons[m_selectedWeaponIdx]->setActive(true);
    }
}
