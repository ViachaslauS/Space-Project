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

    const char* textureBackPath = "bars/bar_bg.png";
    const char* textureEmblem = "hud/level_emblem.png";

    const float EmblemScale = 0.8f;

    const Rectangle bgRectPopup
    {
        0.0f, 0.0f,
        114.0f, 24.0f
    };

    const NPatchInfo NPatch
    {
       .source = bgRectPopup,
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
    m_emblemTexture = LoadTexture(textureEmblem);
}

void HUDPlayerXP::update(float dt)
{
    //
}

void HUDPlayerXP::render()
{
    const Vector2 pos = getConvertedPos();

    const PlayerStats::XPInfo xpInfo = PlayerStats::get().getXPInfo();
    DrawLevelProgress({ pos.x + NPatch.left, pos.y + 70 }, xpInfo.currentXP / PlayerStats::get().getLevelUpXpCost());

    const Vector2 backPos = DrawBack(pos);

    char lvlNum[10]{};
    sprintf(lvlNum, "%02i", xpInfo.currentLvl);

    Rectangle imgRect{
        backPos.x,
        backPos.y,

        m_emblemTexture.width * EmblemScale, 
        m_emblemTexture.height * EmblemScale + 20.0f
    };
    const Vector2 textPos = helpers::getDrawPosInRectCenter(imgRect,lvlNum,FontSize);
   
    Color textColor;
    textColor.a = 255;
    textColor.r = 103;
    textColor.g = 30;
    textColor.b = 50;

    DrawText(lvlNum, textPos.x, textPos.y, FontSize, textColor);
}

void HUDPlayerXP::reset()
{
}

Vector2 HUDPlayerXP::DrawBack(Vector2 pos)
{
    Vector2 resPos = 
    {
        pos.x - m_emblemTexture.width * 0.5f * EmblemScale,
        pos.y - m_emblemTexture.height * 0.6f * EmblemScale
    };
    DrawTextureEx(m_emblemTexture, resPos, 
        0.0f, EmblemScale, WHITE);

    return resPos;
}

void HUDPlayerXP::DrawLevelProgress(Vector2 pos, float progress)
{
    const Rectangle rectDestBack
    {
        pos.x - TargetSize.x * 0.5f - 5, pos.y,
        TargetSize.x, TargetSize.y
    };

    const Rectangle progressBackDest
    {
         pos.x - TargetSize.x * 0.5f - 5, pos.y,
        rectDestBack.width, rectDestBack.height
    };

    const Vector2 originOffset = { rectDestBack.width * 0.5f, rectDestBack.height * 0.5f };

    // draw xp
    DrawRectangle(rectDestBack.x + NPatch.left, rectDestBack.y + NPatch.top,
        (rectDestBack.width * progress - NPatch.right * 2),
        TargetSize.y - NPatch.top * 2, XPColor);

    DrawTextureNPatch(m_progressTexture, NPatch, progressBackDest, {}, 0.0f, WHITE);

}
