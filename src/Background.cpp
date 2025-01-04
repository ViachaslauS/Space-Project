#include "Background.h"

#include <cmath>
#include <Helpers.h>

namespace
{
    constexpr float MoveSpeed = -0.02f;

    constexpr float InitialSpeedSpread = 0.01f;

    static_assert(MoveSpeed < 0.0f && InitialSpeedSpread > 0.0f && MoveSpeed + InitialSpeedSpread < 0.0f);

    const char* PlanetSpriteSheetName = "CelestialObjects.png";

    constexpr uint32_t PlanetSpriteSize = 64u;

    constexpr uint32_t PlanetRows = 3u;
    constexpr uint32_t PlanetColumns = 4u;

    constexpr float StartXOffset = 100.0f;
    constexpr float StartXOfssetSpread = 50.0f;

    constexpr float BaseScale = 1.5f;
    constexpr float ScaleSpread = 1.0f;

    constexpr float SpawnPlanetDelay = 20.0f;
    constexpr float SpawnPlanetDelaySpread = 19.0f;
}

Background::Background()
{
    SetRandomSeed(GetTime());

    m_backgroundTexture = LoadTexture("stars_background.jpg");
    m_planetsTexture = LoadTexture(PlanetSpriteSheetName);

    for (size_t i = 0u; i < m_planets.size(); ++i)
    {
        resetPlanet(m_planets[i]);
    }
}

void Background::update(float dt)
{
    for (Planet& planet : m_planets)
    {
        if (planet.startDelay > 0.0f)
        {
            planet.startDelay -= dt;
            continue;
        }

        planet.relativePos.x += planet.velocity.x * dt;
        planet.relativePos.y += planet.velocity.y * dt;

        if (planet.relativePos.x <= -0.15f)
        {
            resetPlanet(planet);
        }
    }
}

void Background::render()
{
    DrawTexture(m_backgroundTexture, 0, 0, WHITE);

    for (size_t i = 0u; i < m_planets.size(); ++i)
    {
        Rectangle rect;
        rect.x = (m_planets[i].planetTextureNum % PlanetColumns) * PlanetSpriteSize;
        rect.y = (m_planets[i].planetTextureNum / PlanetColumns) * PlanetSpriteSize;

        rect.width = PlanetSpriteSize;
        rect.height = PlanetSpriteSize;

        const Vector2 pos = {
            m_planets[i].relativePos.x * GetScreenWidth(),
            m_planets[i].relativePos.y * GetScreenHeight(),
        };

        Rectangle rectScaled = rect;
        rectScaled.width = rect.width * m_planets[i].scale;
        rectScaled.height = rect.height * m_planets[i].scale;
        rectScaled.x = pos.x - rectScaled.width * 0.5f;
        rectScaled.y = pos.y - rectScaled.height * 0.5f;


        DrawTexturePro(m_planetsTexture, rect, rectScaled, {}, 0.0f, GRAY);
    }
}

void Background::resetPlanet(Planet& planetToReset)
{
    const float ScaleMultiplier = helpers::randFlt();
    planetToReset.scale = helpers::lerpWithDeviation(BaseScale, ScaleSpread, ScaleMultiplier);

    planetToReset.planetTextureNum = GetRandomValue(0, PlanetRows * PlanetColumns - 1u);

    // add velocity based on planet scale
    planetToReset.velocity.x = MoveSpeed * (ScaleMultiplier + 0.5f );
    planetToReset.velocity.y = 0.0f;

    planetToReset.relativePos.x = 1.2f;


    // do not spawn planets near player ship
    const float rndRelPosY = helpers::randFlt();
    const float rndTrueRelPosY = helpers::randFlt();
    planetToReset.relativePos.y = rndRelPosY <= 0.5f ? rndTrueRelPosY * 0.3f : 1.0f - rndTrueRelPosY * 0.3f;;

    planetToReset.startDelay = helpers::lerpWithDeviation(SpawnPlanetDelay, SpawnPlanetDelaySpread, helpers::randFlt());
}
