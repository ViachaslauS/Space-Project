#pragma once

#include "raylib.h"

#include <array>

class Background final
{
    static constexpr uint32_t MaxPlanetsOnScreen = 6u;

    struct Planet
    {
        Vector2 relativePos{};
        Vector2 velocity{};

        uint32_t planetTextureNum{};

        float scale{};

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
    Texture m_planetsTexture;

    std::array<Planet, MaxPlanetsOnScreen> m_planets;
};