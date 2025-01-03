// by Slava

#pragma once

#include <type_traits>

#include "Helpers.h"

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

    helpers::MulticastDelegate<const XPInfo&> onXPChanged;

private:
    PlayerStats();

private:
    XPInfo m_xp;

};