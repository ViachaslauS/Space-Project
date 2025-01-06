#include "HUD/HUDPlayerState.hpp"

#include "Game.h"

#include "external/raygui.h"
#include "Weapons/BaseWeapon.h"
#include <memory>

#include <raymath.h>

namespace
{
    const char* bgWeaponEmptyPath = "hud/weapon_empty.png";
    const char* bgWeaponFullPath = "hud/weapon_full.png";
    const char* weaponSelectedPath = "hud/select.png";

    const Vector2 DebugRectWeapon = { 92.0f, 106.0f };
    const float DistanceBetweenWeapon = 16.0f;

    const int FontSize = 28;

    constexpr int ProgressWidth = 48 * 3.25;
    constexpr int ProgressHeight = 16 * 3.25;

    constexpr int TextureColumns = 7;
    constexpr int TextureRows = 15;

    constexpr int ProgressImgNumBack = 7 * 4;
    constexpr int ProgressImgNumFullHP = 7 * 4 + 1;
    constexpr int ProgressImgNumFullShield = 7 * 6 + 1;

    constexpr float TextureSizeX = 150.0f;
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

    m_weaponEmpty = LoadTexture(bgWeaponEmptyPath);
    m_weaponFull = LoadTexture(bgWeaponFullPath);

    m_weaponSelected = LoadTexture(weaponSelectedPath);
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
        info.weaponTexture = weapon->getWeaponIcon();
        info.isSelectable = weapon->isManualControlAvailable();

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

    int selectableNums = 1;

    for (int idx = 0; idx < m_weapons.size(); idx++)
    {
        const Vector2 offset =
        {
            startPos.x + (DebugRectWeapon.x + DistanceBetweenWeapon) * idx,
            startPos.y
        };

        Texture bgToRender;

        if (m_weapons[idx].isActive || (!m_weapons[idx].isEmpty && m_weapons[idx].isSelectable))
        {
            assert(m_weapons[idx].isSelectable);
            bgToRender = m_weaponFull;
        }
        else
        {
            bgToRender = m_weaponEmpty;
        }

        DrawTexture(bgToRender, offset.x, offset.y, WHITE);

        const Vector2 weaponPos = Vector2Scale(Vector2Add({ (float)bgToRender.width, (float)bgToRender.height }, 
            { (float)-m_weapons[idx].weaponTexture.width, (float)-m_weapons[idx].weaponTexture.height }), 0.5f);


        DrawTexture(m_weapons[idx].weaponTexture, offset.x + weaponPos.x, offset.y + weaponPos.y, WHITE);
    
        if (m_weapons[idx].isActive)
        {
            const Vector2 selectorPos = Vector2Scale(Vector2Add({ (float)bgToRender.width, (float)bgToRender.height },
                { (float)-m_weaponSelected.width, (float)-m_weaponSelected.height }), 0.5f);

            DrawTexture(m_weaponSelected, offset.x + selectorPos.x, offset.y + selectorPos.y, WHITE);
        }
    
        if (m_weapons[idx].isSelectable)
        {
            char num[3] = {};
            sprintf(num, "%d", selectableNums++);
            DrawText(num, offset.x + DebugRectWeapon.x - FontSize * 0.1f, offset.y + DebugRectWeapon.y - FontSize * 0.5f, FontSize, WHITE);
        }
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
       TextureSizeX, ProgressHeight* TextureScaleY
    };

    const float HpWidth = SizeBack.x * (vitality.params.maxHp / VitalityParams{}.maxHp);
    const float ShieldWidth = SizeBack.x * (vitality.params.shieldParams.maxShield / ShieldParams{}.maxShield);

    Rectangle rectHpBackDest{ hpBarPos.x, hpBarPos.y, HpWidth, SizeBack.y };
    Rectangle rectShieldBackDest{ shieldBarPos.x, shieldBarPos.y, ShieldWidth, SizeBack.y };

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
        hpBarPos.x, hpBarPos.y, HpWidth * hpProgress, ProgressHeight * TextureScaleY
    };

    const Rectangle rectProgressShieldDest
    {
        shieldBarPos.x, shieldBarPos.y, ShieldWidth * shieldProgress, ProgressHeight * TextureScaleY
    };

    DrawTextureNPatch(m_progressTexture, BackNpatch, rectHpBackDest, {}, 0.0f, WHITE);
    DrawTextureNPatch(m_progressTexture, HpNpatch, rectHpBackDest, {}, 0.0f, WHITE);

    DrawTextureNPatch(m_progressTexture, BackNpatch, rectProgressShieldDest, {}, 0.0f, WHITE);
    DrawTextureNPatch(m_progressTexture, ShieldNPatch, rectProgressShieldDest, {}, 0.0f, WHITE);

}