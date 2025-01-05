#pragma once

#include "HUD/HUDGameState.hpp"
#include "HUD/HUDPlayerState.hpp"
#include "HUD/HUDPlayerXP.hpp"

#include <vector>

class HUDContainer : public HUDBase
{
public:
    HUDContainer(Game& game);

    virtual void init(Vector2 relPos) override;

    virtual void update(float dt) override;
    virtual void render() override;

    virtual void reset() override;

private:
    void registerChild(HUDBase& child, Vector2 relPos);

private:
    std::vector<HUDBase*> m_children;

    HUDGameState m_gameState;
    HUDPlayerState m_playerState;
    HUDPlayerXP m_playerXp;
};