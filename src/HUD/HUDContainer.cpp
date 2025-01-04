#include "HUD/HUDContainer.hpp"

HUDContainer::HUDContainer(Game& game)
    : HUDBase(game)
    , m_gameState(game)
{
}

void HUDContainer::init(Vector2 /*relPos*/)
{
    HUDBase::init({ 0.0f, 0.0f });

    // add all children here via registerChild()

    registerChild(m_gameState, { 0.8f, 0.05f });
}

void HUDContainer::update(float dt)
{
    HUDBase::update(dt);

    for (auto& child : m_children)
    {
        child->update(dt);
    }
}

void HUDContainer::render()
{
    HUDBase::render();

    for (auto& child : m_children)
    {
        child->render();
    }
}

void HUDContainer::reset()
{
    HUDBase::reset();

    for (auto& child : m_children)
    {
        child->reset();
    }
}

void HUDContainer::registerChild(HUDBase& child, Vector2 relPos)
{
    child.init(relPos);
    m_children.push_back(&child);
}
