#include "HUD/HUDGameState.hpp"

#include "PlayerStats.h"
#include "Game.h"

#include "external/raygui.h"

namespace
{
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

    const Color ProgressColor
    {
        173, 65, 3,
        255
    };
}

HUDGameState::HUDGameState(Game& game)
    : HUDBase(game)
{
    m_texture = LoadTexture(textureBackPath);
}

void HUDGameState::update(float dt)
{
    m_time += dt;
}

void HUDGameState::render()
{
    renderTime();
    renderDifficulty();
}

void HUDGameState::reset()
{
    m_time = 0.0f;
}

void HUDGameState::renderTime()
{
    const int32_t minutes = m_time / 60.0f;
    const float seconds = std::floor(m_time - minutes * 60.0f);

    Vector2 pos = getConvertedPos();
    pos.x += 80;

    char out[20] = {};
    sprintf(out, "%02i:%02.0f\n", minutes, seconds);

    DrawText(out, pos.x, pos.y, 50, WHITE);
}

void HUDGameState::renderDifficulty()
{
    const Vector2 pos = getConvertedPos();
    const GameplayManager& gameplayManager = m_game.getGameplayManager();

    const uint32_t currDiff = gameplayManager.getCurrDifficulty() + 1u;
    float diffProgress = gameplayManager.getDifficultyProgress();

    DrawText("Stage", pos.x, pos.y, 20, WHITE);

    char stageNum[20] = {};
    sprintf(stageNum, "%02i\n", currDiff);

    DrawText(stageNum, pos.x + 20, pos.y + 20, 20, WHITE);

    Vector2 progressPos;
    progressPos.x = pos.x + 2;
    progressPos.y = pos.y + 50;

    Rectangle rect
    {
        progressPos.x, progressPos.y,
        TargetSize.x, TargetSize.y
    };

    DrawRectangle(progressPos.x + NPatch.left, progressPos.y + NPatch.top, TargetSize.x * diffProgress - NPatch.right * 2, TargetSize.y - NPatch.top * 2, ProgressColor);
    DrawTextureNPatch(m_texture, NPatch, rect, {}, 0.0f, WHITE);
}
