// by Slava

#pragma once

#include <type_traits>

#include "Helpers.h"
#include "Skills.h"

class Skills;

class PlayerStats final
{
    friend class Skills;

public:
    static PlayerStats& get();

    struct XPInfo
    {
        void reset()
        {
            currentLvl = 1u;
            currentXP = 0.0f;
        }

        float currentXP = 0.0f;
        uint32_t currentLvl = 1u;
    };

    void reset();
    
    void addXP(float XP);
    XPInfo getXPInfo() const;
    float getLevelUpXpCost() const;

    int getSkillsToUpgradeMax() const;

    helpers::MulticastDelegate<const XPInfo&> onXPChanged;
    helpers::MulticastDelegate<> onLevelUp;

private:
    PlayerStats();

private:
    float m_xpMultiplier = 1.0f;

    int m_skillsToUpgradeCount = 2;

    XPInfo m_xp;
};