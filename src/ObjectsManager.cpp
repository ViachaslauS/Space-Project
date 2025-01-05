#include "ObjectsManager.h"
#include "Physics.hpp"

#include <algorithm>
#include <iterator>

ObjectsManager::ObjectsManager(Physics &physics)
 : m_physics(physics) {}

void ObjectsManager::addObject(const GameObject* obj)
{
    m_objects.push_back(obj);
}

void ObjectsManager::deleteObject(const GameObject* obj)
{
    auto it = std::find(m_objects.begin(), m_objects.end(), obj);
    if (it != m_objects.end())
    {
        if ((*it)->m_physicsComp != nullptr) {
            m_physics.removeBody((*it)->m_physicsComp);
        }
        std::swap(*it, m_objects.back());
        m_objects.pop_back();
    }
}

const std::vector<const GameObject*> ObjectsManager::getEnemyObjects(int teamId) const
{
    std::vector<const GameObject*> result;
    std::copy_if(m_objects.begin(), m_objects.end(), std::back_inserter(result), [teamId](const auto object) {
        return object->getTeamId() != teamId;
    });
    return result;
}

Physics& ObjectsManager::getPhysics()
{
    return m_physics;
}