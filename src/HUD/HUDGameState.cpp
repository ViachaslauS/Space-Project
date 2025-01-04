#include "HUD/HUDGameState.hpp"

#include "PlayerStats.h"
#include "Game.h"

#include "external/raygui.h"

namespace
{
    constexpr int ProgressWidth = 48;
    constexpr int ProgressHeight = 16;

    constexpr int TextureColumns = 7;
    constexpr int TextureRows = 15;

    constexpr int ProgressImgNumBack = 0;
    constexpr int ProgressImgNumFull = 8;

    constexpr float TextureSizeX = 200.0f;
    constexpr float TextureScaleY = 2.5f;
}

HUDGameState::HUDGameState(Game& game)
    : HUDBase(game)
{
    m_texture = LoadTexture("progress.png");
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
    progressPos.x = pos.x + 5;
    progressPos.y = pos.y + 50;

    Rectangle rectBack
    {
        ProgressWidth * (ProgressImgNumBack % TextureColumns),
        ProgressHeight * (ProgressImgNumBack / TextureColumns),

        ProgressWidth,
        ProgressHeight
    };

    Rectangle RectDestBack
    {
        progressPos.x,
        progressPos.y,
        TextureSizeX,
        ProgressHeight * TextureScaleY
    };

    Rectangle rectProgress
    {
        ProgressWidth * (ProgressImgNumFull % TextureColumns),
        ProgressHeight * (ProgressImgNumFull / TextureColumns),

        ProgressWidth * diffProgress,
        ProgressHeight
    };

    Rectangle RectDestProgress
    {
        progressPos.x,
        progressPos.y,
        TextureSizeX * diffProgress,
        ProgressHeight * TextureScaleY
    };

    DrawTexturePro(m_texture, rectBack, RectDestBack, {}, 0.0f, WHITE);
    DrawTexturePro(m_texture, rectProgress, RectDestProgress, {}, 0.0f, WHITE);
}
