#include "Game.h"

Game::Game()
{
    m_background = LoadTexture("stars_background.jpg");
}

void Game::update(float dt)
{

}

void Game::render()
{
    DrawTexture(m_background, 0, 0, WHITE);
}