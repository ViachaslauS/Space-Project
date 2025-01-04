#include "GameplayManager.h"

#include "Enemies/SmallEnemyShip.h"
#include "Enemies/Asteroid.h"

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
    for (auto obj : m_spawnedObjects)
    {
        obj->update(dt);
    }
}

void GameplayManager::render()
{
    for (auto obj : m_spawnedObjects)
    {
        obj->render();
    }
}

uint32_t GameplayManager::getCurrDifficulty() const
{
    return m_currDifficulty;
}

float GameplayManager::getDifficultyProgress() const
{
    return m_difficultyProgress / DifficultyUpgradeTime;
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

    int iterations = 0;
    while (eventToSpawn > MaxEventsCount)
    {
        if (++iterations > 10)
        {
            TraceLog(LOG_ERROR, "[GameplayManager]: Maybe there are endless loop, let's get out of here quickly");
            break;
        }
        eventToSpawn = std::abs(static_cast<int32_t>(MaxEventsCount) * 2 - (eventToSpawn));
    }

    if (eventToSpawn == MaxEventsCount)
    {
        eventToSpawn--;
    }

    Vector2 spawnEventPos = events_helper::getEventSpawnPos(static_cast<EventType>(eventToSpawn));

    return spawnNewObject(static_cast<EventType>(eventToSpawn), spawnEventPos);

}

bool GameplayManager::spawnNewObject(EventType type, Vector2 pos)
{
    switch (type)
    {
        case EventType::SpawnSmallEnemyShip:
        {
            auto newShip = new SmallEnemyShip();
            newShip->setPosition(pos);
            newShip->setSpeed(Vector2 { 20, 0 });
            newShip->initialize();
            m_spawnedObjects.push_back(newShip);
            return true;
        }
        case EventType::Boss:
        {
            return true;
        }
        case EventType::SpawnBigEnemyShip:
        {
            return false;
        }
        case EventType::SpawnDummyAsteroid:
        {
            //Add speed logic
            auto newAsteroid = new Asteroid();
            newAsteroid->setPosition(pos);
            newAsteroid->setSpeed(Vector2 { -30, 0 });
            newAsteroid->initialize();
            m_spawnedObjects.push_back(newAsteroid);
            return true;
        }
        case EventType::SpawnEvilAsteroid:
        {
            //ADD speed logic
            auto newAsteroid = new Asteroid();
            newAsteroid->setPosition(pos);
            newAsteroid->setSpeed(Vector2 { -30, 0 });
            newAsteroid->initialize();
            m_spawnedObjects.push_back(newAsteroid);
            return true;
        }
        case EventType::SpawnLoot:
        {
            return false;
        }
        default:
        {
            return false;
        }
    }
}