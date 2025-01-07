#pragma once

#include "HUD/HUDBase.hpp"

class HUDPlayerXP : public HUDBase
{
public:
    HUDPlayerXP(Game& game);

    virtual void update(float dt) override;
    virtual void render() override;

    virtual void reset() override;

protected:
    Vector2 DrawBack(Vector2 pos);
    void DrawLevelProgress(Vector2 pos, float progress);

protected:
    Texture m_progressTexture;
    Texture m_emblemTexture;
};