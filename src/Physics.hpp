#pragma once

#include <memory>
#include <vector>

#include "box2d/box2d.h"

#include "raylib.h"

class GameObject;
struct Physics;

struct PhysicsComp
{
    Vector2 gravityZoneForce;
    GameObject *object;
    b2ShapeId shapeId;
    b2BodyId id;
    Physics *physics;
};

struct Physics
{
    Physics();
    ~Physics();

    PhysicsComp* createRectangularBody(const Vector2 &center, float width, float height, GameObject *object, bool canRotate = false);
    PhysicsComp* createCircularBody(const Vector2 &center, float radius, GameObject *object);

    void setVelocity(PhysicsComp *comp, const Vector2 &velocity);
    Vector2 getVelocity(PhysicsComp *comp);
    float getRotation(PhysicsComp *comp);

    void applyForce(PhysicsComp *comp, const Vector2 &dir);

    bool removeBody(PhysicsComp *comp);
    void update();
    void debugRender();
    void toggleDebugRender();

    std::vector<std::unique_ptr<PhysicsComp>> comps;

    bool debugDraw = false;

    struct B2d;
    std::unique_ptr<B2d> b2d;
};
