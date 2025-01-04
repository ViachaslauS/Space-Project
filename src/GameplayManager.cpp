#include "GameplayManager.h"

#include "GameEventsSystem.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <random>
#include <functional>
#include <raylib.h>

namespace
{
    constexpr float DifficultyUpgradeTime = 30.0f;

    constexpr uint32_t MaxEventsCount = static_cast<uint32_t>(EventType::Count);

    float getEventSpawnCooldown(uint32_t currDifficulty)
    {
        const float currProgress = static_cast<float>(std::clamp(currDifficulty, 0u, 10u)) * 0.1f;
        return std::lerp(5.0f, 1.0f, currProgress);
    }
}

GameplayManager::GameplayManager()
    : m_currDifficulty(0)
    , m_difficultyProgress(0.0f)
    , m_lastEventSawnTime(0.0f)
    , m_rd()
    , m_rdGen(m_rd())
{
    m_rdGen.seed(static_cast<uint32_t>(GetTime()));
}

void GameplayManager::update(float dt)
{
    updateDifficulty(dt);
    updateEvents(dt);
}

void GameplayManager::render()
{

}

void GameplayManager::updateDifficulty(float dt)
{
    m_difficultyProgress += dt;
    if (m_difficultyProgress >= DifficultyUpgradeTime)
    {
        m_currDifficulty++;
        m_difficultyProgress -= DifficultyUpgradeTime;
    }
}

void GameplayManager::updateEvents(float dt)
{
    m_lastEventSawnTime += dt;
    if (m_lastEventSawnTime >= getEventSpawnCooldown(m_currDifficulty))
    {
        if (spawnEvent())
        {
            m_lastEventSawnTime = 0.0f;
        }
    }
}

bool GameplayManager::spawnEvent()
{
    const int32_t clampedEvent = std::clamp(m_currDifficulty, 0u, static_cast<int32_t>(EventType::Count) - 1u);

    std::normal_distribution<float> normDist(clampedEvent, static_cast<float>(EventType::Count) * 0.5f);

    int32_t eventToSpawn = static_cast<int32_t>(std::abs(normDist(m_rdGen)));

    while (eventToSpawn >= MaxEventsCount)
    {
        eventToSpawn = std::abs(static_cast<int32_t>(MaxEventsCount) * 2 - (eventToSpawn));
    }

    Vector2 SpawnEventPos = events_helper::getEventSpawnPos(static_cast<EventType>(eventToSpawn));

    // TOOD: spawn logic

    return false;
}
