#pragma once

#include "GameObject.h"

struct Physics;

class ObjectsManager final
{
public:
    ObjectsManager(Physics &physics);

    void addObject(const GameObject* obj);
    void deleteObject(const GameObject* obj);

    const std::vector<const GameObject*> getObjectsByTeamId(int teamId) const;

private:
    Physics &m_physics;
    std::vector<const GameObject*> m_objects;
};
