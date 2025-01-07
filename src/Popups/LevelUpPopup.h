#pragma once 

#include "Popups/Popup.h"
#include "Skills.h"

#include <vector>

class LevelUpPopup : public Popup
{
public:
    LevelUpPopup();

    struct PanelInfo
    {
        Skills::SkillProp prop{};

        Rectangle rectData{};
        bool bSelected = false;

        float scaleProgress = 0.0f;
        float currScale = 0.0f;
    };

    void update(float dt) override;
    void render() override;
    void onStateChanged() override;

private:
    Vector2 getPanelSize() const;

    void updateScale(float dt, PanelInfo& panel);

    bool isMouseIn(Rectangle rect);

private:
    Texture m_bgTexture;
    Texture m_selTexture;

    std::vector<PanelInfo> m_infos;
};