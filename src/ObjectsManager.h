#pragma once

#include "GameObject.h"

class ObjectsManager final
{
private:
    ObjectsManager();

public:
    static ObjectsManager& get();

    void addObject(const GameObject* obj);
    void deleteObject(const GameObject* obj);

    const std::vector<const GameObject*> getObjectsByTeamId(int teamId) const;

private:
    std::vector<const GameObject*> m_objects;
};