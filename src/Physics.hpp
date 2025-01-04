#pragma once

#include <memory>
#include <vector>

#include "box2d/box2d.h"

struct Vector2;
class GameObject;

struct PhysicsComp
{
    GameObject *object;
    b2ShapeId shapeId;
    b2BodyId id;
};

struct Physics
{
    Physics();
    ~Physics();

    PhysicsComp* createRectangularBody(const Vector2 &center, float width, float height, GameObject *object);
    PhysicsComp* createCircularBody(const Vector2 &center, float radius, GameObject *object);

    bool removeBody(PhysicsComp *comp);
    void update();
    void debugRender();
    void toggleDebugRender();

    std::vector<std::unique_ptr<PhysicsComp>> comps;

    bool debugDraw = false;

    struct B2d;
    std::unique_ptr<B2d> b2d;
};
