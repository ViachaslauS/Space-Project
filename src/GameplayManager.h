#pragma once

#include <random>
#include <type_traits>

class GameplayManager
{
public:
    GameplayManager();

    void update(float dt);
    void render();

private:
    void updateDifficulty(float dt);

    void updateEvents(float dt);
    bool spawnEvent();

private:
    uint32_t m_currDifficulty;
    float m_difficultyProgress;

    float m_lastEventSawnTime;

    mutable std::random_device m_rd;
    mutable std::mt19937 m_rdGen;
};