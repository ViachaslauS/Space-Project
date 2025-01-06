#include "HUD/HUDPlayerXP.hpp"

#include "Game.h"
#include "PlayerStats.h"

#include "external/raygui.h"
#include <memory>

#include <raymath.h>

namespace
{
    const int FontSize = 50;

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

    const char* textureBackPath = "bars/bar_bg.png";

    const Rectangle bgRect
    {
        0.0f, 0.0f,
        114.0f, 24.0f
    };

    const NPatchInfo NPatch
    {
       .source = bgRect,
       .left = 6,
       .top = 6,
       .right = 6,
       .bottom = 6,
       .layout = NPATCH_NINE_PATCH
    };

    const Vector2 TargetSize
    {
        200.0f, 24.0f
    };

    const Color XPColor
    {
        12, 125, 81,
        255
    };
}

HUDPlayerXP::HUDPlayerXP(Game& game)
    : HUDBase(game)
{
    m_progressTexture = LoadTexture(textureBackPath);
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
    DrawLevelProgress({ pos.x - TargetSize.x * 0.5f + NPatch.left, pos.y + 70 }, xpInfo.currentXP / PlayerStats::get().getLevelUpXpCost());

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
    const Rectangle rectDestBack
    {
        pos.x, pos.y,
        TargetSize.x, TargetSize.y
    };

    const Rectangle progressBackDest
    {
        pos.x, pos.y,
        rectDestBack.width, rectDestBack.height
    };

    const Vector2 originOffset = { rectDestBack.width * 0.5f, rectDestBack.height * 0.5f };

    // draw xp
    DrawRectangle(rectDestBack.x + NPatch.left, rectDestBack.y + NPatch.top,
        (rectDestBack.width * progress - NPatch.right * 2),
        TargetSize.y - NPatch.top * 2, XPColor);

    DrawTextureNPatch(m_progressTexture, NPatch, progressBackDest, {}, 0.0f, WHITE);

}
