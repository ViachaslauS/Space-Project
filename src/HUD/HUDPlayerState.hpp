#pragma once

#include "HUD/HUDBase.hpp"

#include <vector>

class HUDPlayerState : public HUDBase
{
public:
    HUDPlayerState(Game& game);

    virtual void update(float dt) override;
    virtual void render() override;

    virtual void reset() override;

protected:
    void renderWeapons();
    void renderVitality();

protected:
    struct WeaponInfo
    {
        bool isEmpty = true;
        bool isActive = false;
        bool isSelectable = false;

        Texture weaponTexture;
    };

    std::vector<WeaponInfo> m_weapons;

    Texture m_progressTexture;

    Texture m_weaponEmpty;
    Texture m_weaponFull;
    Texture m_weaponSelected;
};