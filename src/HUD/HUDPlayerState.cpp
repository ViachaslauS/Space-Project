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
    const char* weaponReloadPath = "bars/weapon_reload.png";
    const char* numBackPath = "hud/back_for_num.png";

    const Vector2 DebugRectWeapon = { 92.0f, 106.0f };
    const float DistanceBetweenWeapon = 16.0f;

    const int FontSize = 20;

    const char* textureBackPath = "bars/bar_bg.png";

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

    const Color ShieldColor
    {
        64, 144, 168,
        255
    };

    const Color HpColor
    {
        156, 9, 41,
        255
    };

    const Rectangle WeaponReloadFillRect
    {
        6, 4,
        80, 2
    };

    const Color WeaponReloadColor
    {
       40, 230, 167,
       255
    };
}

HUDPlayerState::HUDPlayerState(Game& game)
    : HUDBase(game)
{
    m_progressTexture = LoadTexture(textureBackPath);

    m_weaponEmpty = LoadTexture(bgWeaponEmptyPath);
    m_weaponFull = LoadTexture(bgWeaponFullPath);

    m_weaponSelected = LoadTexture(weaponSelectedPath);
    m_weaponReload = LoadTexture(weaponReloadPath);

    m_numBack = LoadTexture(numBackPath);
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
        info.reloadProgress = weapon->getReloadProgress();

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
        auto& weapon = m_weapons[idx];
        const Vector2 offset =
        {
            startPos.x + (DebugRectWeapon.x + DistanceBetweenWeapon) * idx,
            startPos.y
        };

        Texture bgToRender;

        if (weapon.isActive || (!weapon.isEmpty && weapon.isSelectable))
        {
            assert(weapon.isSelectable);
            bgToRender = m_weaponFull;
        }
        else
        {
            bgToRender = m_weaponEmpty;
        }

        DrawTexture(bgToRender, offset.x, offset.y, WHITE);

        const Vector2 weaponPos = Vector2Scale(Vector2Add({ (float)bgToRender.width, (float)bgToRender.height },
            { (float)-weapon.weaponTexture.width, (float)-weapon.weaponTexture.height }), 0.5f);


        DrawTexture(weapon.weaponTexture, offset.x + weaponPos.x, offset.y + weaponPos.y, WHITE);

        if (weapon.isActive)
        {
            const Vector2 selectorPos = Vector2Scale(Vector2Add({ (float)bgToRender.width, (float)bgToRender.height },
                { (float)-m_weaponSelected.width, (float)-m_weaponSelected.height }), 0.5f);

            DrawTexture(m_weaponSelected, offset.x + selectorPos.x, offset.y + selectorPos.y, WHITE);
        }

        const Vector2 reloadPos
        {
            offset.x, offset.y + DebugRectWeapon.y
        };

        DrawTexture(m_weaponReload, reloadPos.x, reloadPos.y, WHITE);
        DrawRectangle(reloadPos.x + WeaponReloadFillRect.x,
            reloadPos.y + WeaponReloadFillRect.y,
            WeaponReloadFillRect.width* weapon.reloadProgress,
            WeaponReloadFillRect.height, WeaponReloadColor
        );

        if (weapon.isSelectable)
        {
            const Rectangle backRect = {
                offset.x - m_numBack.width * 0.5f + bgToRender.width,
                offset.y - m_numBack.height * 0.9f + bgToRender.height,
                (float)m_numBack.width, (float)m_numBack.height
            };

            DrawTexture(m_numBack, backRect.x, backRect.y, WHITE);

            std::string num = std::to_string(selectableNums++);

            const Vector2 textPos = helpers::getDrawPosInRectCenter(backRect, num, FontSize);
            DrawText(num.c_str(), textPos.x, textPos.y, FontSize, WHITE);
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

    Vector2 shieldBarPos = { pos.x, pos.y + DebugRectWeapon.y + 20.f };
    Vector2 hpBarPos = { pos.x, pos.y + TargetSize.y + DebugRectWeapon.y + 25.0f };

    const Vector2 SizeBack = TargetSize;

    const float HpWidth = SizeBack.x * (vitality.params.maxHp / VitalityParams{}.maxHp);
    const float ShieldWidth = SizeBack.x * (vitality.params.shieldParams.maxShield / ShieldParams{}.maxShield);

    Rectangle rectShield
    {
        shieldBarPos.x, shieldBarPos.y,
        ShieldWidth, TargetSize.y
    };
    // draw shield
    DrawRectangle(shieldBarPos.x + NPatch.left, shieldBarPos.y + NPatch.top,
        (ShieldWidth * shieldProgress - NPatch.right * 2),
        TargetSize.y - NPatch.top * 2, ShieldColor);
    DrawTextureNPatch(m_progressTexture, NPatch, rectShield, {}, 0.0f, WHITE);

    Rectangle rectHp
    {
        hpBarPos.x, hpBarPos.y,
        HpWidth, TargetSize.y
    };

    // draw hp
    DrawRectangle(hpBarPos.x + NPatch.left, hpBarPos.y + NPatch.top,
        (HpWidth * hpProgress - NPatch.right * 2),
        TargetSize.y - NPatch.top * 2, HpColor);

    DrawTextureNPatch(m_progressTexture, NPatch, rectHp, {}, 0.0f, WHITE);
}
