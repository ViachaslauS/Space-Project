#pragma once

#include <array>
#include <vector>

#include "Weapons/BaseWeapon.h"

class Game;

class Skills
{
public:
    enum class SkillEnum : int
    {
        XPGainMultiplier,

        GetGravigun,
        GravigunPower,
        GravigunDamageSpeed,
        GravizoneLifetime,
        GravizoneSize,

        HPCapacity,
        ShieldCapacity,

        GetRocket,
        RocketDamage,
        RocketDamageSpeed,

        GetLaser,
        LaserDamage,
        LaserDamageSpeed,

        FullRegen,
        AdditionalSkillVariant,

        Count,
    };

    struct Mapping
    {
        SkillEnum skill;
        const char* desc;
    };

    static constexpr Mapping Mappings[] {
        { SkillEnum::XPGainMultiplier, "Increase\nXP Gain" },
        { SkillEnum::GetGravigun, "Get additional\ngravigun" },
        { SkillEnum::GravigunPower, "Increase power\nof gravigun" },
        { SkillEnum::GravigunDamageSpeed, "Decrease\ngravigun cooldown" },
        { SkillEnum::GravizoneLifetime, "Increase\ngravizone lifetime" },
        { SkillEnum::GravizoneSize, "Increase\ngravizone size" },

        { SkillEnum::HPCapacity, "Increase maximum\nHP\n(and heals\nyou completely!)" },
        { SkillEnum::ShieldCapacity, "Increase maximum\nshield\n(and restore\nit fully!)" },

        { SkillEnum::GetRocket, "Get additional\nmissile gun" },
        { SkillEnum::RocketDamage, "Increase missile\ngun damage" },
        { SkillEnum::RocketDamageSpeed, "Decrease missile\ngun cooldown" },

        { SkillEnum::GetLaser, "Get additional\nblaster gun" },
        { SkillEnum::LaserDamage, "Increase blaster\ngun damage" },
        { SkillEnum::LaserDamageSpeed, "Decrease blaster\ngun cooldown" },

        { SkillEnum::FullRegen, "Fully restores\nyour HP\nand shield" },
        { SkillEnum::AdditionalSkillVariant, "Add one\nmore variant\nto\nchoose skill" },
    };

    struct SkillProp
    {
        Skills::SkillEnum skill;
        uint32_t level;

        const char* desc;
    };

    using Props = std::vector<SkillProp>;

    const char* getSkillDesc(SkillEnum skill);

    Props getAvailableToUpgrade(Game& game);

    bool isSkillAvailabeToUpgrade(Game& game, SkillEnum skill);

    void learnPlayerSkill(Game& game, SkillEnum skill, int newLevel);

    const Props& getProps() const;

    void resetSkills(Game& game);

private:
    SkillProp& getSkillProp(SkillEnum skillType);

    bool haveWeapon(Game& game, WeaponType weaponType) const;

    std::vector<BaseWeapon*> getAllWeaponsByType(Game& game, WeaponType weaponType) const;

    void upgradeWeapon(Game& game, WeaponType weaponType);

private:
    Props m_skills;
};
