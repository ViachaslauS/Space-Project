#pragma once

#include "raylib.h"

#include <array>
#include <vector>

class Background final
{
    static constexpr uint32_t MaxPlanetsOnScreen = 3u;

    struct Planet
    {
        Vector2 relativePos{};
        Vector2 velocity{};

        uint32_t planetTextureNum{};

        float scale{};
        float rotation{};

        float startDelay{};
    };

public:
    Background();

    void update(float dt);
    void render();

private:
    static void resetPlanet(Planet& planetToReset);

private:
    Texture m_backgroundTexture;
    std::vector<Texture> m_planetsTexture;

    std::array<Planet, MaxPlanetsOnScreen> m_planets;
};