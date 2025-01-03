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
    bool isImmortal = false;
    float maxHp = 100.0f;

    bool hasShield = false;
    ShieldParams shieldParams;
};

struct VitalityData
{
    VitalityData(const VitalityParams& params)
        : currentHP(params.maxHp)
        , currentShield(params.hasShield ? params.shieldParams.maxShield : -1.0f)
    {}

    float currentHP = 0.0f;
    float currentShield = 0.0f;

    float lastDamadeReceivedDelay = 0.0f;
};