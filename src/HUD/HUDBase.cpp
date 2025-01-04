#include "HUD/HUDBase.hpp"

#include "Game.h"

#include <raymath.h>

HUDBase::HUDBase(Game& game)
    : m_game(game)
{
}

void HUDBase::init(Vector2 relPos)
{
    m_relPos = relPos;
}

void HUDBase::update(float dt)
{
}

void HUDBase::render()
{
}

void HUDBase::reset()
{
}

Vector2 HUDBase::getConvertedPos() const
{
    return Vector2Multiply(m_relPos, { (float)GetScreenWidth(), (float)GetScreenHeight() });
}
