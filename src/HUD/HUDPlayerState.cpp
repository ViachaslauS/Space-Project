#include "HUD/HUDPlayerState.hpp"

#include "Game.h"

#include "external/raygui.h"
#include "Weapons/BaseWeapon.h"
#include <memory>

#include <raymath.h>

namespace
{
    const Vector2 DebugRectWeapon = { 64.0f, 64.0f };
    const float DistanceBetweenWeapon = 16.0f;

    const int FontSize = 28;

    constexpr int ProgressWidth = 48 * 3.25;
    constexpr int ProgressHeight = 16 * 3.25;

    constexpr int TextureColumns = 7;
    constexpr int TextureRows = 15;

    constexpr int ProgressImgNumBack = 7 * 4;
    constexpr int ProgressImgNumFullHP = 7 * 4 + 1;
    constexpr int ProgressImgNumFullShield = 7 * 6 + 1;

    constexpr float TextureSizeX = 420.0f;
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
}

HUDPlayerState::HUDPlayerState(Game& game)
    : HUDBase(game)
{
    m_progressTexture = LoadTexture("progress.png");
}

void HUDPlayerState::update(float dt)
{
    m_weapons.clear();
    m_weapons.reserve(m_game.getPlayerShip().getMaxWeapons());

    const auto& shipWeapons = m_game.getPlayerShip().getWeapons();

    for (const auto& weapon : shipWeapons)
    {
        WeaponInfo info;
        info.isActive = weapon->isActiveWeapon();
        info.isEmpty = false;
        info.weaponTexture = weapon->getWeaponTexture();

        m_weapons.push_back(info);
    }

    while (m_weapons.size() < m_game.getPlayerShip().getMaxWeapons())
    {
        m_weapons.push_back(WeaponInfo{});
    }
}

void HUDPlayerState::render()
{
    renderWeapons();
    renderVitality();
}

void HUDPlayerState::reset()
{
}

void HUDPlayerState::renderWeapons()
{
    const Vector2 startPos = getConvertedPos();

    for (int idx = 0; idx < m_weapons.size(); idx++)
    {
        const Vector2 offset =
        {
            startPos.x + (DebugRectWeapon.x + DistanceBetweenWeapon) * idx,
            startPos.y
        };

        if (m_weapons[idx].isActive)
        {
            Color activeColor = GREEN;
            activeColor.a = 200;

            DrawRectangle(offset.x, offset.y, DebugRectWeapon.x, DebugRectWeapon.y, activeColor);
        }

        // draw bg
        Color bgColor = RED;
        bgColor.a = 150;
        DrawRectangle(offset.x, offset.y, DebugRectWeapon.x, DebugRectWeapon.y, bgColor);

        char num[3] = {};
        sprintf(num, "%d", idx + 1);
        DrawText(num, offset.x + DebugRectWeapon.x - FontSize * 0.1f, offset.y + DebugRectWeapon.y - FontSize * 0.5f, FontSize, WHITE);
    }
}

void HUDPlayerState::renderVitality()
{
    const Vector2 pos = getConvertedPos();

    const auto& ship = m_game.getPlayerShip();
    const auto vitality = ship.getVitality();

    const float hpProgress = vitality.data.currentHP / vitality.params.maxHp;
    const float shieldProgress = vitality.data.currentShield / vitality.params.shieldParams.maxShield;

    Vector2 shieldBarPos = { pos.x - 15.0f, pos.y + DebugRectWeapon.y + 10.f };
    Vector2 hpBarPos = { pos.x - 15.0f, pos.y + DebugRectWeapon.y + ProgressHeight * TextureScaleY + 2.0f };

    const Vector2 SizeBack
    {
       TextureSizeX, ProgressHeight * TextureScaleY
    };

    Rectangle rectHpBackDest{ hpBarPos.x, hpBarPos.y, SizeBack.x, SizeBack.y };
    Rectangle rectShieldBackDest{ shieldBarPos.x, shieldBarPos.y, SizeBack.x, SizeBack.y };

    const Rectangle rectProgressHp
    {
        ProgressWidth * (ProgressImgNumFullHP % TextureColumns),
        ProgressHeight * (ProgressImgNumFullHP / TextureColumns),

        ProgressWidth * hpProgress,
        ProgressHeight
    };

    NPatchInfo HpNpatch
    { 
        .source = rectProgressHp,
        .left = 32,
        .top = 0,
        .right = 32,
        .bottom = 0,
        .layout = NPATCH_NINE_PATCH
    };

    const Rectangle rectProgressShield
    {
        ProgressWidth * (ProgressImgNumFullShield % TextureColumns),
        ProgressHeight * (ProgressImgNumFullShield / TextureColumns),

        ProgressWidth * shieldProgress,
        ProgressHeight
    };

    NPatchInfo ShieldNPatch
    {
        .source = rectProgressShield,
        .left = 32,
        .top = 0,
        .right = 32,
        .bottom = 0,
        .layout = NPATCH_NINE_PATCH
    };

    const Rectangle rectProgressHpDest
    {
        hpBarPos.x, hpBarPos.y, TextureSizeX * hpProgress, ProgressHeight * TextureScaleY
    };

    const Rectangle rectProgressShieldDest
    {
        shieldBarPos.x, shieldBarPos.y, TextureSizeX * shieldProgress, ProgressHeight * TextureScaleY
    };

    DrawTextureNPatch(m_progressTexture, BackNpatch, rectHpBackDest, {}, 0.0f, WHITE);
    DrawTextureNPatch(m_progressTexture, HpNpatch, rectHpBackDest, {}, 0.0f, WHITE);

    DrawTextureNPatch(m_progressTexture, BackNpatch, rectProgressShieldDest, {}, 0.0f, WHITE);
    DrawTextureNPatch(m_progressTexture, ShieldNPatch, rectProgressShieldDest, {}, 0.0f, WHITE);

}