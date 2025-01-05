#include "Popup.h"

Popup::Popup()
{

}

void Popup::show(bool show)
{
    m_isVisible = show;
    onStateChanged();
}

void Popup::update(float dt)
{

}

void Popup::render()
{
    
}

void Popup::onStateChanged()
{

}

void Popup::setGame(Game* game)
{
    m_game = game;
}
