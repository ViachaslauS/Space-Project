#include <algorithm>

#include "Popups/LevelUpPopup.h"

#include "external/raygui.h"
#include "raymath.h"
#include "PlayerStats.h"

#include "external/reasings.h"

#include "Game.h"

namespace
{
    const char* bgTextureName = "popup/window.png";
    const char* bgPathButtonSelected = "popup/window_selected.png";

    const Rectangle bgItemRect =
    {
        0.0f, 0.0f,
        458.0f, 166.0f
    };

    NPatchInfo bgNpatchPopup = {
        .source = bgItemRect,
        .left = 37,
        .top = 34,
        .right = 40,
        .bottom = 36,
        .layout = NPATCH_NINE_PATCH
    };

    const Vector2 relBgScale = { 0.28f, 0.35f };
    const float relDistBetween = 0.04f;
  
    const Rectangle bgRectButtonSelected
    {
        0, 0,
        387.0f, 102.0f
    };

    const NPatchInfo bgNpatchButtonSelected
    {
        .source = bgRectButtonSelected,
        .left = 41,
        .top = 35,
        .right = 41,
        .bottom = 35,
        .layout = NPATCH_NINE_PATCH
    };

    const float ScaleTime = 0.5f;
}

LevelUpPopup::LevelUpPopup()
    : Popup()
{
    setType(PopupType::LevelUpPopup);

    m_bgTexture = LoadTexture(bgTextureName);
    m_selTexture = LoadTexture(bgPathButtonSelected);
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

            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && panel.scaleProgress >= ScaleTime)
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

        DrawTextureNPatch(m_bgTexture, bgNpatchPopup, panel.rectData, { 0.0f, 0.0f }, 0.0f, WHITE);

        constexpr float FontSize = 30;
        const char* TEMPLATE = "TEMPLATE STRING";
        Vector2 textVec = helpers::getDrawPosInRectCenter(panel.rectData, TEMPLATE, FontSize);
        textVec.y -= FontSize * 2.0f;
        
        DrawText(panel.prop.desc, textVec.x, textVec.y, FontSize, WHITE);
    
        if (panel.bSelected)
        {
            Rectangle selectedRect
            {
                panel.rectData.x + bgNpatchPopup.left,
                panel.rectData.y + bgNpatchPopup.top,

                 panel.rectData.width - (bgNpatchPopup.left + bgNpatchPopup.right),
                 panel.rectData.height - (bgNpatchPopup.top + bgNpatchPopup.bottom),

            };
            DrawTextureNPatch(m_selTexture, bgNpatchButtonSelected,
                selectedRect, { }, 0.0f, WHITE);
        }
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
