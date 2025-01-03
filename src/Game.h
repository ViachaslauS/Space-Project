#pragma once

#include "raylib.h"

class Game
{
public:
    Game();

public:
    void update(float dt);
    void render();

private:
    Texture m_background;
};