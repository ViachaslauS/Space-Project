// by Slava

#pragma once

struct ShieldParams
{
    float maxShield = 100.0f;

    float startRegenDelay = 2.0f;

    float regenPerSec = 5.0f;
};

struct VitalityParams
{
    bool bImmortal = false;
    float maxHp = 100.0f;

    bool bHasShield = false;
    ShieldParams shieldParams;
};

struct VitalityData
{
    VitalityData(const VitalityParams& params)
        : currentHP(params.maxHp)
        , currentShield(params.bHasShield ? params.shieldParams.maxShield : -1.0f)
    {}

    float currentHP = 0.0f;
    float currentShield = 0.0f;

    float lastDamadeReceivedDelay = 0.0f;
};