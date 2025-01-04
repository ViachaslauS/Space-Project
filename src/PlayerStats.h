// by Slava

#pragma once

#include <type_traits>

#include "Helpers.h"
#include "Skills.h"

class PlayerStats final
{
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

    struct SkillProp
    {
        Skills::Skills skill;
        uint32_t currLevel;
        uint32_t maxLevel;
    };
    std::vector<SkillProp>& getSkills();

    helpers::MulticastDelegate<const XPInfo&> onXPChanged;
    helpers::MulticastDelegate<> onLevelUp;

private:
    PlayerStats();

private:
    XPInfo m_xp;
    std::vector<SkillProp> m_skillsProp;
};