#pragma once

#include <memory>
#include <vector>

#include "box2d/box2d.h"

#include "GameObject.h"

struct Vector2;

struct PhysicsComp
{
    ObjectType type;
    b2BodyId id;
};

struct Physics
{
    Physics();
    ~Physics();

    PhysicsComp createRectangularBody(const Vector2 &center, float width, float height, GameObject *object);
    PhysicsComp createCircularBody(const Vector2 &center, float radius, GameObject *object);

    bool removeBody(const PhysicsComp &comp);
    void update();
    void debugRender();
    void toggleDebugRender();

    std::vector<PhysicsComp> comps;

    bool debugDraw = false;

    struct B2d;
    std::unique_ptr<B2d> b2d;
};
