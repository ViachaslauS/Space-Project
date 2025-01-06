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
    m_xp.reset();
}

void PlayerStats::addXP(float XP)
{
    m_xp.currentXP += XP * m_xpMultiplier;

    while (m_xp.currentXP >= getLevelUpXpCost())
    {
        m_xp.currentXP -= getLevelUpXpCost();
        ++m_xp.currentLvl;
        onLevelUp.broadcast();
    }

    onXPChanged.broadcast(m_xp);
}

PlayerStats::XPInfo PlayerStats::getXPInfo() const
{
    return m_xp;
}

float PlayerStats::getLevelUpXpCost() const
{
    return XPPerLvl * m_xp.currentLvl;
}

int PlayerStats::getSkillsToUpgradeMax() const
{
    return m_skillsToUpgradeCount;
}
