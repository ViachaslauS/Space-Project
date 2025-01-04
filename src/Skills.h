#pragma once

#include <vector>
namespace Skills
{
    enum class Skills
    {
        XPGain,
        GravigunPower,
        ShieldCapacity,
        XPGainRadius,

        Count,
    };

    struct Mapping
    {
        Skills skill;
        const char* name;
    };

    static std::vector<Mapping> Mappings = {
        { Skills::XPGain, "Increase XP Gain" },
        { Skills::GravigunPower, "Increase Power of Gravigun" },
        { Skills::ShieldCapacity, "Increase Shield capacity" },
        { Skills::XPGainRadius, "Increase XP gain radius" },
    };

    static const char* getSkillName(Skills skill)
    {
        for (const auto& map : Mappings)
        {
            if (map.skill == skill)
            {
                return map.name;
            }
        }
        return "";
    }
}