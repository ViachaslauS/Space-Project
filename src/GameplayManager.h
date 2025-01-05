#pragma once

#include <random>
#include <type_traits>

#include "GameObject.h"
#include "GameEventsSystem.h"

struct Physics;
class PlayerShip;

class GameplayManager
{
public:
    GameplayManager(Physics &p, ObjectsManager &om, PlayerShip* playerShip);

    Physics &m_physics;
    ObjectsManager &m_objectManager;

    void update(float dt);
    void render();

    uint32_t getCurrDifficulty() const;
    float getDifficultyProgress() const;

private:
    void updateDifficulty(float dt);

    void updateEvents(float dt);
    bool spawnEvent();

    bool spawnNewObject(EventType type, Vector2 pos);

    Vector2 calculateVelocityToPlayer(const Vector2& pos, bool withOffset);

    void deleteSpawnedObject(GameObject* obj);

private:
    uint32_t m_currDifficulty;
    float m_difficultyProgress;

    float m_lastEventSawnTime;

    std::vector<std::unique_ptr<GameObject>> m_spawnedObjects;
    std::vector<GameObject*> m_needToRemoveObjects;
    PlayerShip* m_playerShip;

    mutable std::random_device m_rd;
    mutable std::mt19937 m_rdGen;
};
