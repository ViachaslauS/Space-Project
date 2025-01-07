#include "Background.h"

#include <cmath>
#include <Helpers.h>

namespace
{
    constexpr float MoveSpeed = -0.02f;

    constexpr float InitialSpeedSpread = 0.01f;

    static_assert(MoveSpeed < 0.0f && InitialSpeedSpread > 0.0f && MoveSpeed + InitialSpeedSpread < 0.0f);

    const std::array<const char*, 2> PlanetPaths = {
        "planet_1.png", 
        "planet_2.png"
    }; 
    
    constexpr float StartXOffset = 100.0f;
    constexpr float StartXOfssetSpread = 50.0f;

    constexpr float BaseScale = 0.3f;
    constexpr float ScaleSpread = 0.2f;

    constexpr float SpawnPlanetDelay = 10.0f;
    constexpr float SpawnPlanetDelaySpread = 7.0f;
}

Background::Background()
{
    SetRandomSeed(GetTime());

    m_backgroundTexture = LoadTexture("background_2.png");

    m_planetsTexture.reserve(PlanetPaths.size());
    for (int i = 0; i < PlanetPaths.size(); i++)
    {
        Texture planetTexture = LoadTexture(PlanetPaths[i]);
        m_planetsTexture.push_back(planetTexture);
    }

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
        const auto& planet = m_planets[i];
        const Vector2 pos = {
            m_planets[i].relativePos.x * GetScreenWidth(),
            m_planets[i].relativePos.y * GetScreenHeight(),
        };

        if (helpers::isValidIdx(m_planetsTexture, planet.planetTextureNum))
        {
            DrawTextureEx(m_planetsTexture[planet.planetTextureNum],
                pos, planet.rotation, planet.scale, WHITE);
        }
    }
}

void Background::resetPlanet(Planet& planetToReset)
{
    const float ScaleMultiplier = helpers::randFlt();
    planetToReset.scale = helpers::lerpWithDeviation(BaseScale, ScaleSpread, ScaleMultiplier);

    planetToReset.planetTextureNum = std::rand() % PlanetPaths.size();

    // add velocity based on planet scale
    planetToReset.velocity.x = MoveSpeed * (ScaleMultiplier + 0.5f );
    planetToReset.velocity.y = 0.0f;

    planetToReset.relativePos.x = 1.4f;

    // do not spawn planets near player ship
    const float rndRelPosY = helpers::randFlt();
    const float rndTrueRelPosY = helpers::randFlt();
    planetToReset.relativePos.y = rndRelPosY <= 0.5f ? rndTrueRelPosY * 0.3f : 1.0f - rndTrueRelPosY * 0.3f;

    planetToReset.startDelay = helpers::lerpWithDeviation(SpawnPlanetDelay, SpawnPlanetDelaySpread, helpers::randFlt());

    planetToReset.rotation = helpers::randFlt(0.0f, 360.0f);
}
