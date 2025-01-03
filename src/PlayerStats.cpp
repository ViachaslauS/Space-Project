// by Slava

#include "PlayerStats.h"

namespace
{
    constexpr float XPPerLvl = 500.0f;
}

PlayerStats::PlayerStats()
{
    reset();
}


PlayerStats& PlayerStats::get()
{
    static PlayerStats Stats;

    return Stats;
}

void PlayerStats::reset()
{
    onXPChanged.empty();
    m_xp.reset();
}

void PlayerStats::addXP(float XP)
{
    m_xp.currentXP += XP;

    if (m_xp.currentXP >= XPPerLvl)
    {
        m_xp.currentXP -= XPPerLvl;
        ++m_xp.currentLvl;
    }

    onXPChanged.broadcast(m_xp);
}
