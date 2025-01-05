#include "Popups/LevelUpPopup.h"

#include "external/raygui.h"
#include "raymath.h"
#include "PlayerStats.h"

#include "external/reasings.h"

#include "Game.h"

namespace
{
    const char* bgTextureName = "popups.png";

    const Rectangle bgItemRect = 
    {
        36.0f, 45.0f,
        243.0f, 101.0f
    };

    NPatchInfo bgNpatch = {
        .source = bgItemRect,
        .left = 34,
        .top = 34,
        .right = 102,
        .bottom = 18,
        .layout = NPATCH_NINE_PATCH
    };

    const Vector2 relBgScale = { 0.22f, 0.42f };
    const float relDistBetween = 0.04f;

    const int SelectedYOffset = 524;
    const Rectangle bgItemRectSelected =
    {
        54.0f, SelectedYOffset,
        243.0f, 101.0f
    };

    NPatchInfo bgNpatchSelected = {
        .source = bgItemRectSelected,
        .left = 33,
        .top = 34,
        .right = 102,
        .bottom = 18,
        .layout = NPATCH_NINE_PATCH
    };

    const float ScaleTime = 0.5f;
}

LevelUpPopup::LevelUpPopup()
    : Popup()
{
    setType(PopupType::LevelUpPopup);

    m_bgTexture = LoadTexture(bgTextureName);
}

void LevelUpPopup::update(float dt)
{
    if (!m_game || !m_infos.size())
    {
        show(false);
    }

    const Vector2 overallPanelSize = Vector2Multiply(Vector2AddValue(relBgScale, relDistBetween), { (float)GetScreenWidth(), (float)GetScreenHeight() });
    const Vector2 overallSize{ overallPanelSize.x * m_infos.size(), overallPanelSize.y };
    const float offset = GetScreenWidth() * 0.5f - overallSize.x * 0.5f;

    for (int i = 0; i < m_infos.size(); i++)
    {
        auto& panel = m_infos[i];
        updateScale(dt, panel);

        panel.rectData.width = relBgScale.x * GetScreenWidth() * panel.currScale;
        panel.rectData.height = relBgScale.y * GetScreenHeight() * panel.currScale;

        panel.rectData.x = offset + overallPanelSize.x * i;
        panel.rectData.y = GetScreenHeight() * 0.5f - panel.rectData.height * 0.5f;

        if (isMouseIn(panel.rectData))
        {
            panel.bSelected = true;

            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
            {
                m_game->getSkills().learnPlayerSkill(*m_game, panel.prop.skill, panel.prop.level + 1);
                show(false);
            }
        }
        else
        {
            panel.bSelected = false;
        }
    }
}

void LevelUpPopup::render()
{
    if (!m_game)
    {
        return;
    }

    for (int i = 0; i < m_infos.size(); i++)
    {
        const auto& panel = m_infos[i];
        NPatchInfo targetNPatch = panel.bSelected ? bgNpatchSelected : bgNpatch;

        DrawTextureNPatch(m_bgTexture, targetNPatch, panel.rectData, { 0.0f, 0.0f }, 0.0f, WHITE);
        DrawText(panel.prop.desc, panel.rectData.x + panel.rectData.width * 0.05f, panel.rectData.y + panel.rectData.height * 0.3f, 30, WHITE);
    }
}

void LevelUpPopup::onStateChanged()
{
    m_infos.clear();

    if (!m_game || !isVisible())
    {
        return;
    }

    auto& skills = m_game->getSkills();

    auto allAvailable = skills.getAvailableToUpgrade(*m_game);

    const int maxToUpgrade = PlayerStats::get().getSkillsToUpgradeMax();

    for (int i = 0; i < maxToUpgrade; i++)
    {
        if (allAvailable.empty())
        {
            break;
        }

        int skillIdx = std::rand() % allAvailable.size();

        PanelInfo info;

        info.bSelected = false;
        info.prop = allAvailable[skillIdx];
        info.rectData = bgItemRect;

        std::swap(allAvailable[skillIdx], allAvailable.back());
        allAvailable.pop_back();

        m_infos.push_back(info);
    }
}

Vector2 LevelUpPopup::getPanelSize() const
{
    return Vector2Multiply(Vector2AddValue(relBgScale, relDistBetween), { (float)GetScreenWidth(), (float)GetScreenHeight() });

}

void LevelUpPopup::updateScale(float dt, PanelInfo& panel)
{
    if (panel.scaleProgress < ScaleTime)
    {
        panel.scaleProgress += dt;

        std::clamp(panel.scaleProgress, 0.0f, 1.0f);

        panel.currScale = EaseBackOut(panel.scaleProgress, 0.0f, 1.0f, ScaleTime);
    }
    else
    {
        panel.currScale = 1.0f;
    }
}

bool LevelUpPopup::isMouseIn(Rectangle rect)
{
    return CheckCollisionPointRec(GetMousePosition(), rect);
}
