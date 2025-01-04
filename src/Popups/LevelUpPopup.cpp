#include "Popups/LevelUpPopup.h"

#include "external/raygui.h"
#include "PlayerStats.h"

namespace
{
    static constexpr uint32_t UpgradeSkillsMaxCount = 3;
}

LevelUpPopup::LevelUpPopup()
{
    setType(PopupType::LevelUpPopup);
}

void LevelUpPopup::update(float dt)
{
    if (m_noUpgradeSkills)
    {
        m_noUpgradeSkills = false;
        show(false);
        return;
    }
    if (m_skillUpgradeNum != -1)
    {
        auto& stats = PlayerStats::get();
        auto& skills = stats.getSkills();
        auto it = std::find_if(skills.begin(), skills.end(), [this](auto& skillProp) {
            return m_upgradeSkills[m_skillUpgradeNum] == skillProp.skill;
        });
        if (it != skills.end())
        {
            it->currLevel++;
        }
        m_skillUpgradeNum = -1;
        show(false);
    }
}

void LevelUpPopup::render()
{
    if (m_upgradeSkills.empty())
    {
        m_noUpgradeSkills = true;
        return;
    }
    if (GuiButton((Rectangle { 70, 60, 350, 700}), Skills::getSkillName(m_upgradeSkills[0])))
    {
        m_skillUpgradeNum = 0;
    }
    if (m_upgradeSkills.size() > 1)
    {
        if (GuiButton((Rectangle { 470, 60, 350, 700}), Skills::getSkillName(m_upgradeSkills[1])))
        {
            m_skillUpgradeNum = 1;
        }
        if (m_upgradeSkills.size() > 2)
        {
            if (GuiButton((Rectangle { 870, 60, 350, 700}), Skills::getSkillName(m_upgradeSkills[2])))
            {
                m_skillUpgradeNum = 2;
            }
        }
    }
}

void LevelUpPopup::onStateChanged()
{
    if (isVisible())
    {
        calculateSkills();
    }
}

void LevelUpPopup::calculateSkills()
{
    m_upgradeSkills.clear();
    auto& stats = PlayerStats::get();
    auto& skills = stats.getSkills();
    std::vector<Skills::Skills> skillsCanBeUpgraded;
    for (const auto& skill : skills)
    {
        if (skill.currLevel != skill.maxLevel)
        {
            skillsCanBeUpgraded.push_back(skill.skill);
        }
    }
    if (skillsCanBeUpgraded.empty())
    {
        return;
    }

    const auto upgradeSkillCount = skillsCanBeUpgraded.size() < UpgradeSkillsMaxCount ? skillsCanBeUpgraded.size() : UpgradeSkillsMaxCount;
    for (auto i = 0; i < upgradeSkillCount; i++)
    {
        const auto randSkill = GetRandomValue(0, skillsCanBeUpgraded.size() - 1);
        m_upgradeSkills.push_back(skillsCanBeUpgraded[randSkill]);
    }
}