#include "HUD/HUDPlayerXP.hpp"

#include "Game.h"
#include "PlayerStats.h"

#include "external/raygui.h"
#include <memory>

#include <raymath.h>

namespace
{
    const int FontSize = 50;

    constexpr int ProgressWidth = 48 * 3.25;
    constexpr int ProgressHeight = 16 * 3.25;

    constexpr int TextureColumns = 7;
    constexpr int TextureRows = 15;

    constexpr int ProgressImgNumBack = 7 * 8;
    constexpr int ProgressImgNumFullProgress = 7 * 11 + 1;

    constexpr float TextureSizeX = ProgressWidth;
    constexpr float TextureScaleY = 1.0f;

    constexpr Rectangle rectBack
    {
        ProgressWidth * (ProgressImgNumBack % TextureColumns),
        ProgressHeight * (ProgressImgNumBack / TextureColumns),
        ProgressWidth,
        ProgressHeight
    };

    constexpr NPatchInfo BackNpatch
    {
        .source = rectBack,
        .left = 32,
        .top = 0,
        .right = 32,
        .bottom = 0,
        .layout = NPATCH_NINE_PATCH
    };

    constexpr float NumScale = 3.0f;

    const Rectangle BackWingsRect
    {
        0.0f, 8.0f,
        80.0f, 18.0f
    };

    const Rectangle BackBorderRect
    {
        224.0f, 4.0f, 
        44.0f, 40.0f
    };

    const Rectangle BackShieldRect
    {
        228.0f, 56.0f, 
        36.0f, 32.0f
    };
}

HUDPlayerXP::HUDPlayerXP(Game& game)
    : HUDBase(game)
{
    m_progressTexture = LoadTexture("progress.png");
    m_levelTexture = LoadTexture("level.png");
}

void HUDPlayerXP::update(float dt)
{
    //
}

void HUDPlayerXP::render()
{
    const Vector2 pos = getConvertedPos();

    const PlayerStats::XPInfo xpInfo = PlayerStats::get().getXPInfo();
    DrawLevelProgress({ pos.x, pos.y + 80 }, xpInfo.currentXP / PlayerStats::get().getLevelUpXpCost());

    DrawBack({ pos.x + 4, pos.y }, BackWingsRect);
    DrawBack(pos, BackShieldRect);
    DrawBack(pos, BackBorderRect);


    char lvlNum[10]{};
    sprintf(lvlNum, "%02i", xpInfo.currentLvl);

    Color textColor;
    textColor.a = 255;
    textColor.r = 103;
    textColor.g = 30;
    textColor.b = 50;

    DrawText(lvlNum, pos.x - 20, pos.y - FontSize * 0.35f, FontSize, textColor);
}

void HUDPlayerXP::reset()
{
}

void HUDPlayerXP::DrawBack(Vector2 pos, Rectangle rect)
{
    DrawTexturePro(m_levelTexture, rect
        , { pos.x, pos.y, rect.width * NumScale, rect.height * NumScale }
        , { rect.width * 1.5f, rect.height * 1.5f }, 0.0f, WHITE);
}

void HUDPlayerXP::DrawLevelProgress(Vector2 pos, float progress)
{
    Rectangle rectDestBack
    {
        pos.x, pos.y,
        TextureSizeX, ProgressHeight * TextureScaleY
    };

    const Rectangle rectProgress
    {
        ProgressWidth * (ProgressImgNumFullProgress % TextureColumns),
        ProgressHeight * (ProgressImgNumFullProgress / TextureColumns),

        ProgressWidth * progress,
        ProgressHeight * TextureScaleY
    };

    const Rectangle progressBackDest
    {
        pos.x, pos.y,
        rectProgress.width, rectProgress.height
    };

    const Vector2 originOffset = { rectDestBack.width * 0.5f, rectDestBack.height * 0.5f };

    DrawTexturePro(m_progressTexture, rectBack, rectDestBack, originOffset, 0.0f, WHITE);
    DrawTexturePro(m_progressTexture, rectProgress, progressBackDest, originOffset, 0.0f, WHITE);
}
