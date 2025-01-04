#pragma once

#include "raylib.h"

#include <type_traits>

enum class EventType : uint32_t
{
    SpawnDummyAsteroid, // fly near player ship
    SpawnSmallEnemyShip, // have small amount of hp
    SpawnLoot,
    SpawnEvilAsteroid, // fly directly to player ship
    SpawnBigEnemyShip,
    Boss,

    Count
};

namespace events_helper
{
    Vector2 getEventSpawnPos(EventType eventType);
}