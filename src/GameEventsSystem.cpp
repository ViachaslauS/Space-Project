#include "GameEventsSystem.h"

#include "Helpers.h"

#include <raymath.h>

namespace
{
    constexpr float GameSizeX = 4000.0f; // ??? 
    constexpr float GameSizeY = 4000.0f; // ??? 


    Vector2 RandomRightWindowCorner(float relIndent = 0.0f)
    {
        Vector2 randPos;
        randPos.x = GetScreenWidth() * (1 + relIndent);
        randPos.y = GetScreenHeight() * helpers::randFlt();

        return randPos;
    }

    Vector2 RandomLeftWindowCorner(float relIndent = 0.0f)
    {
        Vector2 randPos;
        randPos.x = -GetScreenWidth() * relIndent;
        randPos.y = GetScreenHeight() * helpers::randFlt();

        return randPos;
    }

    Vector2 RandomRightGameCorner(float relIndent = 0.0f)
    {
        Vector2 randPos;
        randPos.x = GameSizeX * (1 + relIndent);
        randPos.y = GameSizeY * helpers::randFlt();

        return randPos;
    }

    Vector2 RandomLeftGameCorner(float relIndent = 0.0f)
    {
        Vector2 randPos;
        randPos.x = -GameSizeX * relIndent;
        randPos.y = GameSizeY * helpers::randFlt();

        return randPos;
    }
}

Vector2 events_helper::getEventSpawnPos(EventType eventType)
{
    switch (eventType)
    {
    case EventType::SpawnDummyAsteroid:
        return RandomRightWindowCorner(0.1f);
    case EventType::SpawnSmallEnemyShip:
        return RandomLeftWindowCorner(0.1f);
   /* case EventType::SpawnLoot:
        return RandomRightGameCorner();*/
    case EventType::SpawnEvilAsteroid:
        return RandomRightWindowCorner(0.1f);
  /*  case EventType::SpawnBigEnemyShip:
        return RandomLeftWindowCorner();
    case EventType::Boss:
        return RandomLeftWindowCorner();*/
    case EventType::Count:
    default:
        TraceLog(LOG_ERROR, "%s: invalid event type!", __FUNCTION__);
        break;
    }
}
