#include "ObjectsManager.h"

#include <algorithm>
#include <iterator>

ObjectsManager::ObjectsManager()
{
    m_objects.clear();
}

ObjectsManager& ObjectsManager::get()
{
    static ObjectsManager objectsManager;

    return objectsManager;
}

void ObjectsManager::addObject(const GameObject* obj)
{
    m_objects.push_back(obj);
}

void ObjectsManager::deleteObject(const GameObject* obj)
{
    auto it = std::find(m_objects.begin(), m_objects.end(), obj);
    if (it != m_objects.end())
    {
        std::swap(*it, m_objects.back());
        m_objects.pop_back();
    }
}

const std::vector<const GameObject*> ObjectsManager::getObjectsByTeamId(int teamId) const
{
    std::vector<const GameObject*> result;
    std::copy_if(m_objects.begin(), m_objects.end(), std::back_inserter(result), [teamId](const auto object) {
        return object->getTeamId() == teamId;
    });
    return result;
}