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

    m_skillsProp.clear();
    m_skillsProp.reserve(static_cast<size_t>(Skills::Skills::Count));
    for (auto i = 0; i < static_cast<size_t>(Skills::Skills::Count); i++)
    {
        m_skillsProp.push_back({ static_cast<Skills::Skills>(i), 0u, 10u });
    }
}

void PlayerStats::addXP(float XP)
{
    m_xp.currentXP += XP;

    if (m_xp.currentXP >= getLevelUpXpCost())
    {
        m_xp.currentXP -= XPPerLvl;
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

std::vector<PlayerStats::SkillProp>& PlayerStats::getSkills()
{
    return m_skillsProp;
}