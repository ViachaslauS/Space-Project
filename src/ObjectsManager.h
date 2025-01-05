#pragma once

#include "GameObject.h"

struct Physics;

class ObjectsManager final
{
public:
    ObjectsManager(Physics &physics);

    void addObject(const GameObject* obj);
    void deleteObject(const GameObject* obj);

    Physics& getPhysics();

    const std::vector<const GameObject*> getEnemyObjects(int teamId) const;

private:
    Physics &m_physics;
    std::vector<const GameObject*> m_objects;
};
