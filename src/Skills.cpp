#include "Skills.h"

#include "Game.h"
#include "PlayerStats.h"

#include "Weapons/Gravigun.hpp"
#include "Weapons/LaserWeapon.h"
#include "Weapons/RocketWeapon.h"

#include <cassert>


const char* Skills::getSkillDesc(SkillEnum skill)
{
    for (const auto& map : Mappings)
    {
        if (map.skill == skill)
        {
            return map.desc;
        }
    }
    
    assert(false && "missing skill mapping");
    
    return "";
}

Skills::Props Skills::getAvailableToUpgrade(Game& game)
{
    Skills::Props props;

    for (int i = 0; i < (int)SkillEnum::Count; i++)
    {
        if (isSkillAvailabeToUpgrade(game, (SkillEnum)i))
        {
            SkillProp prop = getSkillProp((SkillEnum)i);
            props.push_back(prop);
        }
    }

    return props;
}

bool Skills::isSkillAvailabeToUpgrade(Game& game, SkillEnum skill)
{
    switch (skill)
    {
    case Skills::SkillEnum::XPGainMultiplier:
        return true;
    case Skills::SkillEnum::GetGravigun:
    {
        const auto& playerShip = game.getPlayerShip();
        return playerShip.getWeapons().size() < playerShip.getMaxWeapons();
    }
    case Skills::SkillEnum::GravigunPower:
    {
        return haveWeapon(game, WeaponType::Gravigun);
    }
    case Skills::SkillEnum::GravigunDamageSpeed:
    {
        constexpr int MaxLevel = 10;
        return haveWeapon(game, WeaponType::Gravigun) && getSkillProp(SkillEnum::GravigunDamageSpeed).level < MaxLevel;
    }
    case Skills::SkillEnum::GravizoneLifetime: 
    {
        constexpr int MaxLevel = 10;
        return haveWeapon(game, WeaponType::Gravigun) && getSkillProp(SkillEnum::GravizoneLifetime).level < MaxLevel;
    }
    case Skills::SkillEnum::GravizoneSize:
    {
        constexpr int MaxLevel = 10;
        return haveWeapon(game, WeaponType::Gravigun) && getSkillProp(SkillEnum::GravizoneSize).level < MaxLevel;
    }
    case Skills::SkillEnum::HPCapacity:
    {
        constexpr int MaxLevel = 10;
        return getSkillProp(SkillEnum::HPCapacity).level < MaxLevel;
    }
    case Skills::SkillEnum::ShieldCapacity:
    {
        constexpr int MaxLevel = 10;
        return getSkillProp(SkillEnum::ShieldCapacity).level < MaxLevel;
    }
    case Skills::SkillEnum::GetRocket:
    {
        const auto& playerShip = game.getPlayerShip();
        return playerShip.getWeapons().size() < playerShip.getMaxWeapons();
    }
    case Skills::SkillEnum::RocketDamage:
        return haveWeapon(game, WeaponType::RocketLauncher);
    case Skills::SkillEnum::RocketDamageSpeed:
    {
        constexpr int MaxLevel = 10;
        return haveWeapon(game, WeaponType::RocketLauncher) && getSkillProp(SkillEnum::RocketDamageSpeed).level < MaxLevel;
    }
    case Skills::SkillEnum::GetLaser:
    {
        const auto& playerShip = game.getPlayerShip();
        return playerShip.getWeapons().size() < playerShip.getMaxWeapons();
    }
    case Skills::SkillEnum::LaserDamage:
        return haveWeapon(game, WeaponType::Laser);
    case Skills::SkillEnum::LaserDamageSpeed:
    {
        constexpr int MaxLevel = 3;
        return haveWeapon(game, WeaponType::Laser) && getSkillProp(SkillEnum::LaserDamageSpeed).level < MaxLevel;
    }
    case Skills::SkillEnum::FullRegen:
        return true;
    case Skills::SkillEnum::AdditionalSkillVariant:
        return getSkillProp(SkillEnum::AdditionalSkillVariant).level < 1;
    case Skills::SkillEnum::Count:
    default:
        assert(false && "undefined skill");
        break;
    }
}

void Skills::learnPlayerSkill(Game& game, SkillEnum skill, int newLevel)
{
    assert(isSkillAvailabeToUpgrade(game, skill));

    auto& prop = getSkillProp(skill);
    prop.level = newLevel;

    switch (skill)
    {
    case Skills::SkillEnum::XPGainMultiplier:
    {
        PlayerStats::get().m_xpMultiplier = 1.0f + 0.1f * prop.level;
        break;
    }
    case Skills::SkillEnum::GetGravigun:
    {
        if (prop.level == 0)
        {
            break;
        }
        auto& playerShip = game.m_playerShip;
        playerShip.addWeapon(new Gravigun(playerShip.m_objectManager, playerShip.getTeamId(), playerShip.m_gravityZones));

        break;
    }
    case Skills::SkillEnum::GravigunPower:
    {
        upgradeWeapon(game, WeaponType::Gravigun);
        break;
    }
    case Skills::SkillEnum::GravigunDamageSpeed:
    {
        upgradeWeapon(game, WeaponType::Gravigun);
        break;
    }
    case Skills::SkillEnum::GravizoneLifetime:
        game.m_gravityZones.params.gzLifetime = GravityZoneSystem::GZParams{}.gzLifetime + prop.level * 0.5f;

        break;
    case Skills::SkillEnum::GravizoneSize:
        game.m_gravityZones.params.width = GravityZoneSystem::GZParams{}.width + prop.level * 0.25f;
        game.m_gravityZones.params.height = GravityZoneSystem::GZParams{}.height + prop.level * 0.25f;

        break;
    case Skills::SkillEnum::HPCapacity:
    {
        game.m_playerShip.m_vitality.maxHp = VitalityParams{}.maxHp + 0.1f * prop.level;
        game.m_playerShip.m_vitalityData.currentHP = game.m_playerShip.m_vitality.maxHp;

        break; 
    }
    case Skills::SkillEnum::ShieldCapacity:
    {
        game.m_playerShip.m_vitality.shieldParams.maxShield = ShieldParams{}.maxShield + 0.2f * prop.level;
        game.m_playerShip.m_vitalityData.currentShield = game.m_playerShip.m_vitality.shieldParams.maxShield;
        break;
    }
    case Skills::SkillEnum::GetRocket:
    { 
        if (prop.level == 0)
        {
            break;
        }
        auto& playerShip = game.m_playerShip;
        playerShip.addWeapon(new RocketWeapon(playerShip.m_objectManager, playerShip.getTeamId()));

        break;
    }
    case Skills::SkillEnum::RocketDamage:
    {
        upgradeWeapon(game, WeaponType::RocketLauncher);
        break;
    }
    case Skills::SkillEnum::RocketDamageSpeed:
    {
        upgradeWeapon(game, WeaponType::RocketLauncher);
        break;
    }
    case Skills::SkillEnum::GetLaser: 
    {
        if (prop.level == 0)
        {
            break;
        }
        auto& playerShip = game.m_playerShip;
        playerShip.addWeapon(new LaserWeapon(playerShip.m_objectManager, playerShip.getTeamId()));

        break;
    }
    case Skills::SkillEnum::LaserDamage:
    {
        upgradeWeapon(game, WeaponType::Laser);
        break;
    }
    case Skills::SkillEnum::LaserDamageSpeed:
    {
        upgradeWeapon(game, WeaponType::Laser);
        break;
    }
    case Skills::SkillEnum::FullRegen:
        if (prop.level == 0) break;
        game.m_playerShip.m_vitalityData.currentShield = game.m_playerShip.m_vitality.shieldParams.maxShield;
        game.m_playerShip.m_vitalityData.currentShield = game.m_playerShip.m_vitality.shieldParams.maxShield;
        break;
    case Skills::SkillEnum::AdditionalSkillVariant:
        PlayerStats::get().m_skillsToUpgradeCount = prop.level > 0 ? 3 : 2;
        break;
    case Skills::SkillEnum::Count:
        break;
    default:
        break;
    }
}

const Skills::Props& Skills::getProps() const
{
    return m_skills;
}

void Skills::resetSkills(Game& game)
{
    for (int i = 0; i < (int)SkillEnum::Count; i++)
    {
        learnPlayerSkill(game, (SkillEnum)i, 0);
    }
}

Skills::SkillProp& Skills::getSkillProp(SkillEnum skillType)
{
    if (skillType == SkillEnum::Count)
    {
        assert(false && "r u serious?");
        static SkillProp emptyProp;

        return emptyProp;
    }

    for (auto& skill : m_skills)
    {
        if (skill.skill == skillType)
        {
            return skill;
        }
    }
    
    SkillProp newSkill;
    newSkill.desc = getSkillDesc(skillType);
    newSkill.level = 0;
    newSkill.skill = skillType;
    
    m_skills.push_back(newSkill);

    return m_skills.back();
}

bool Skills::haveWeapon(Game& game, WeaponType weaponType) const
{
    return getAllWeaponsByType(game, weaponType).size() > 0;
}

std::vector<BaseWeapon*> Skills::getAllWeaponsByType(Game& game, WeaponType weaponType) const
{
    std::vector<BaseWeapon*> outWeapon;
    for (auto& weapon : game.getPlayerShip().getWeapons())
    {
        if (weapon->getWeaponType() == weaponType)
        {
            outWeapon.push_back(weapon.get());
        }
    }

    return outWeapon;
}

void Skills::upgradeWeapon(Game& game, WeaponType weaponType)
{
    auto param = game.m_playerShip.getParamForWeaponType(weaponType);

    SkillEnum cooldownSkill = SkillEnum::Count;
    SkillEnum damageSkill = SkillEnum::Count;

    switch (weaponType)
    {
    case WeaponType::Gravigun:
        cooldownSkill = SkillEnum::GravigunDamageSpeed;
        damageSkill = SkillEnum::GravigunPower;
        break;
    case WeaponType::Laser:
        cooldownSkill = SkillEnum::LaserDamageSpeed;
        damageSkill = SkillEnum::LaserDamage;
        break;
    case WeaponType::RocketLauncher:
        cooldownSkill = SkillEnum::RocketDamageSpeed;
        damageSkill = SkillEnum::RocketDamage;
        break;
    case WeaponType::Count:
        break;
    default:
        break;
    }

    param.weaponCooldown = GetDefaultParam(weaponType).weaponCooldown / (1 + getSkillProp(cooldownSkill).level);
    param.weaponDamage = GetDefaultParam(weaponType).weaponCooldown + getSkillProp(damageSkill).level;

    game.m_playerShip.applyWeaponParam(weaponType, param);
}
