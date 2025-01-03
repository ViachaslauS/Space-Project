#include "Popups.h"

#include "Popup.h"
#include <algorithm>

Popups::Popups()
{   
}

void Popups::update(float dt)
{
    for (auto popup : m_popups)
    {
        if (popup->isVisible())
        {
            popup->update(dt);
        }
    }
}

void Popups::render()
{
    for (auto popup : m_popups)
    {
        if (popup->isVisible())
        {
            popup->render();
        }
    }
}

bool Popups::isCoveredPopup()
{
    auto hasCovered = std::any_of(m_popups.cbegin(), m_popups.cend(), [](auto popup) {
        return popup->isVisible();
    });

    return hasCovered;
}

void Popups::addPopup(Popup* popup)
{
    m_popups.push_back(popup);
}

void Popups::killPopup(Popup* popup)
{
    auto it = std::find(m_popups.begin(), m_popups.end(), popup);

    if (it != m_popups.end())
    {
        std::swap(*it, m_popups.back());
        m_popups.pop_back();
    }
}

Popup* Popups::getPopup(PopupType type)
{
    auto it = std::find_if(m_popups.begin(), m_popups.end(), [type](auto popup) {
        return popup->getType() == type;
    });
    if (it != m_popups.end())
    {
        return *it;
    }
    else
    {
        return nullptr;
    }
}