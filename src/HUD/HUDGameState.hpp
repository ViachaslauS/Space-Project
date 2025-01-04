#pragma once

#include "HUD/HUDBase.hpp"

class HUDGameState : public HUDBase
{
public:
    HUDGameState(Game& game);

    virtual void update(float dt) override;
    virtual void render() override;

    virtual void reset();

protected:
    void renderTime();
    void renderDifficulty();

protected:
    float m_time = 0.0f;
    Texture m_texture;
};