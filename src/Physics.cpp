#include <cassert>

#include "box2d/box2d.h"
#include "utils.h"

#include "GameObject.h"
#include "GravityZone.hpp"
#include "Physics.hpp"

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>

namespace rlgl {
    #include "rlgl.h"
}

struct Physics::B2d
{
    b2WorldId worldId;
    std::vector<b2BodyId> bodyIds;
    b2DebugDraw dDraw;
};

namespace
{
    void drawPolygon(const b2Vec2 *vertices, int vertexCount, b2HexColor color, void *)
    {
        if (vertexCount <= 0) {
            // what
            TraceLog(LOG_ERROR, "Zero vertices in shape passed to debug draw");
            return;
        }

        std::vector<Vector2> points;
        for (int i = 0; i < vertexCount; i++)
        {
            points.push_back({ vertices[i].x, vertices[i].y });
        }
        Color c = {
                (uint8_t)(color & 0xff0000),
                (uint8_t)(color & 0x00ff00),
                (uint8_t)(color & 0x0000ff),
                0xff, };
        DrawLineStrip(points.data(), vertexCount, c);
    }

    void drawSolidPolygon(b2Transform transform, const b2Vec2* vertices, int vertexCount, float radius, b2HexColor color,
                          void* context)
    {
        if (vertexCount <= 0) {
            TraceLog(LOG_ERROR, "Zero vertices in shape passed to debug draw");
            return;
        }

        std::vector<Vector2> points;
        for (int i = 0; i < vertexCount; i++)
        {
            auto newVert = b2TransformPoint(transform, vertices[i]);
            points.push_back({ newVert.x, newVert.y });
        }
        Color c = {
                (uint8_t)(color & 0xff0000),
                (uint8_t)(color & 0x00ff00),
                (uint8_t)(color & 0x0000ff),
                0xff, };
        for (auto i = 1; i < points.size(); i++)
        {
            DrawLineEx(points[i - 1], points[i], 2.0f, c);
        }
        DrawLineEx(points.back(), points.front(), 2.0f, c);
    }

    void drawSolidCircle(b2Transform transform, float radius, b2HexColor color, void* context)
    {
        Vector2 center { transform.p.x, transform.p.y };

        Color c = {
                (uint8_t)(color & 0xff0000),
                (uint8_t)(color & 0x00ff00),
                (uint8_t)(color & 0x0000ff),
                0xff, };

        DrawCircleLinesV(center, radius, c);
    }

    void drawSolidCapsule(b2Vec2 p1, b2Vec2 p2, float radius, b2HexColor color, void*)
    {
        b2Vec2 v = b2Sub(p2, p1);
        b2Vec2 vn = b2Normalize(v);
        b2Rot r { vn.x, vn.y };
        float angle = b2Rot_GetAngle(r);
        float step = M_PI / 10.0f;

        rlgl::rlBegin(RL_LINES);
        rlgl::rlColor4ub((uint8_t)(color & 0xff0000),
                   (uint8_t)(color & 0x00ff00),
                   (uint8_t)(color & 0x0000ff),
                   0xff);

        int i = 0;
        for (; i < 10; i += 1)
        {
            rlgl::rlVertex2f(p1.x + cosf(angle + M_PI_2 + step * i) * radius,
                             p1.y + sinf(angle + M_PI_2 + i * step) * radius);
            rlgl::rlVertex2f(p1.x + cosf(angle + M_PI_2 + step * (i + 1)) * radius,
                             p1.y + sinf(angle + M_PI_2 + (i + 1) * step) * radius);
        }
        rlgl::rlVertex2f(p1.x + cosf(angle + M_PI_2 + step * i) * radius,
                         p1.y + sinf(angle + M_PI_2 + i * step) * radius);
        rlgl::rlVertex2f(p2.x + cosf(angle + M_PI_2 - step * i) * radius,
                         p2.y + sinf(angle + M_PI_2 - i * step) * radius);
        for (i = 0; i < 10; i += 1)
        {
            rlgl::rlVertex2f(p2.x + cosf(angle + M_PI_2 - step * i) * radius,
                             p2.y + sinf(angle + M_PI_2 - i * step) * radius);
            rlgl::rlVertex2f(p2.x + cosf(angle + M_PI_2 - step * (i + 1)) * radius,
                             p2.y + sinf(angle + M_PI_2 - (i + 1) * step) * radius);
        }
        rlgl::rlVertex2f(p2.x + cosf(angle + M_PI_2) * radius,
                         p2.y + sinf(angle + M_PI_2) * radius);
        rlgl::rlVertex2f(p1.x + cosf(angle + M_PI_2) * radius,
                         p1.y + sinf(angle + M_PI_2) * radius);
        rlgl::rlEnd();
    }

    b2BodyType objectTypeToBodyType(ObjectType type) {
        switch (type) {
        case ObjectType::GravityZone:
        case ObjectType::PlayerShip:
            return b2BodyType::b2_staticBody;
        case ObjectType::RocketProjectile:
        case ObjectType::LaserProjectile:
        case ObjectType::Asteroid:
        case ObjectType::EnemyShip:
            return b2BodyType::b2_dynamicBody;
        default:
            // uh oh
            TraceLog(LOG_ERROR, "Unknown physics object type");
            assert(false);
        }
    }

    bool isBullet(ObjectType type) {
        return type == ObjectType::LaserProjectile;
    }

    bool isSensor(ObjectType type) {
        return type == ObjectType::GravityZone;
    }

    uint32_t getCategory(int teamId) {
        // team ids       cat
        // 0 player        2
        // 1 enemy         4 (1 << 2)
        // 2 asteroids     8 (1 << 3)
        // category 1 is reserved for technical things like
        // geometry queries
        if (teamId == 0) {
            return 2;
        } else if (teamId == 1) {
            return 1 << 2;
        }
        return 1 << 3;
    }

    uint32_t getMask(int teamId) {
        if (teamId == 0) {
            return 1 | 1 << 2 | 1 << 3;
        } else if (teamId == 1) {
            return 1 | 2 | 1 << 3;
        }
        return 0xFFFFFFFF; // collide with everything
    }

    struct OverlapResult {
        std::vector<GameObject *> intersectedObjects;
    };

    bool processOverlap(b2ShapeId shapeId, void* context) {
        auto r = reinterpret_cast<OverlapResult *>(context);
        auto userData = b2Shape_GetUserData(shapeId);
        r->intersectedObjects.push_back(reinterpret_cast<GameObject *>(userData));
        return true;
    }
}

Physics::~Physics()
{
    b2DestroyWorld(b2d->worldId);
}

Physics::Physics()
    : b2d(std::make_unique<B2d>())
{
    b2d->dDraw.DrawPolygon = &drawPolygon;
    b2d->dDraw.DrawSolidPolygon = &drawSolidPolygon;
    b2d->dDraw.DrawSolidCircle = &drawSolidCircle;
    b2d->dDraw.DrawSolidCapsule = &drawSolidCapsule;

    float lengthUnitsPerMeter = 200.0f;
    b2SetLengthUnitsPerMeter(lengthUnitsPerMeter);

    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity.x = 0.0f;
    worldDef.gravity.y = 0.0f;

    b2d->worldId = b2CreateWorld(&worldDef);
}

PhysicsComp* Physics::createRectangularBody(const Vector2 &pos, float width, float height, GameObject *object, bool canRotate)
{
    b2Vec2 extent = { width / 2.0f, height / 2.0f };
    b2Polygon poly = b2MakeBox(extent.x, extent.y);
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = objectTypeToBodyType(object->m_objectType);
    bodyDef.position = { pos.x, pos.y };
    bodyDef.fixedRotation = !canRotate;
    bodyDef.enableSleep = false;
    bodyDef.isBullet = isBullet(object->m_objectType);

    auto comp = std::make_unique<PhysicsComp>();
    comp->physics = this;
    comp->object = object;
    comp->id = b2CreateBody(b2d->worldId, &bodyDef);

    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.userData = reinterpret_cast<void *>(object);
    shapeDef.isSensor = isSensor(object->m_objectType);
    shapeDef.filter.categoryBits = getCategory(object->getTeamId());
    shapeDef.filter.maskBits = getMask(object->getTeamId());
    comp->shapeId = b2CreatePolygonShape(comp->id, &shapeDef, &poly);

    if (bodyDef.isBullet) {
        auto massData = b2Body_GetMassData(comp->id);
        massData.mass = 0.1f;
        b2Body_SetMassData(comp->id, massData);
    }

    object->setPhysicsComp(comp.get());
    comps.push_back(std::move(comp));
    return comps.back().get();
}

PhysicsComp* Physics::createCircularBody(const Vector2 &center, float radius, GameObject *object)
{
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = objectTypeToBodyType(object->m_objectType);
    bodyDef.position = { center.x, center.y };
    bodyDef.fixedRotation = true;
    bodyDef.enableSleep = false;
    bodyDef.isBullet = isBullet(object->m_objectType);

    auto comp = std::make_unique<PhysicsComp>();
    comp->physics = this;
    comp->object = object;
    comp->id = b2CreateBody(b2d->worldId, &bodyDef);

    b2Vec2 c { 0.0f, 0.0f };
    b2Circle circle { c, radius };
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.userData = reinterpret_cast<void *>(object);
    shapeDef.isSensor = isSensor(object->m_objectType);
    shapeDef.filter.categoryBits = getCategory(object->getTeamId());
    shapeDef.filter.maskBits = getMask(object->getTeamId());
    comp->shapeId = b2CreateCircleShape(comp->id, &shapeDef, &circle);

    if (bodyDef.isBullet) {
        auto massData = b2Body_GetMassData(comp->id);
        massData.mass = 0.1f;
        b2Body_SetMassData(comp->id, massData);
    }

    object->setPhysicsComp(comp.get());
    comps.push_back(std::move(comp));
    return comps.back().get();
}

PhysicsComp* Physics::createCapsuleBody(const Vector2& pos, const Vector2 &center1, const Vector2 &center2, float radius, GameObject *object, bool canRotate)
{
    b2BodyDef bodyDef = b2DefaultBodyDef();
    b2Vec2 center { pos.x, pos.y };
    bodyDef.type = objectTypeToBodyType(object->m_objectType);
    bodyDef.position = center;
    bodyDef.fixedRotation = !canRotate;
    bodyDef.enableSleep = false;
    bodyDef.isBullet = isBullet(object->m_objectType);

    auto comp = std::make_unique<PhysicsComp>();
    comp->physics = this;
    comp->object = object;
    comp->id = b2CreateBody(b2d->worldId, &bodyDef);

    b2Vec2 c1 { center1.x, center1.y };
    b2Vec2 c2 { center2.x, center2.y };
    b2Capsule capsule { c1, c2, radius };
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.userData = reinterpret_cast<void *>(object);
    shapeDef.isSensor = isSensor(object->m_objectType);
    shapeDef.filter.categoryBits = getCategory(object->getTeamId());
    shapeDef.filter.maskBits = getMask(object->getTeamId());
    comp->shapeId = b2CreateCapsuleShape(comp->id, &shapeDef, &capsule);

    if (bodyDef.isBullet) {
        auto massData = b2Body_GetMassData(comp->id);
        massData.mass = 0.1f;
        b2Body_SetMassData(comp->id, massData);
    }

    object->setPhysicsComp(comp.get());
    comps.push_back(std::move(comp));
    return comps.back().get();
}

bool Physics::removeBody(PhysicsComp *comp)
{
    if (comp == nullptr) {
        return true;
    }

    size_t i = 0;
    while (i < comps.size())
    {
        if (B2_ID_EQUALS(comps[i]->id, comp->id))
        {
            b2DestroyBody(comp->id);
            comps[i] = std::move(comps.back());
            comps.pop_back();
            return true;
        }
        i++;
    }

    // no such comp >:(
    TraceLog(LOG_ERROR, "Failed to find PhysicsComp");
    return false;
}

void Physics::update()
{
    auto dt = GetFrameTime();
    b2World_Step(b2d->worldId, dt > 0.016 ? 0.016 : dt, 4);

    auto contactEvents = b2World_GetContactEvents(b2d->worldId);
    for (int i = 0; i < contactEvents.beginCount; ++i)
    {
        b2ContactBeginTouchEvent* beginEvent = contactEvents.beginEvents + i;
        auto obj1 = reinterpret_cast<GameObject *>(b2Shape_GetUserData(beginEvent->shapeIdA));
        auto obj2 = reinterpret_cast<GameObject *>(b2Shape_GetUserData(beginEvent->shapeIdB));
        obj1->onCollision(obj2);
    }

    for (auto &c : comps) {
        auto pos = b2Body_GetPosition(c->id);
        c->object->setPos({ pos.x, pos.y });

        auto rot = b2Body_GetRotation(c->id);
        c->object->setRotation(b2Rot_GetAngle(rot));
    }
}

void Physics::debugRender()
{
    b2World_Draw(b2d->worldId, &b2d->dDraw);
}

void Physics::toggleDebugRender()
{
    b2d->dDraw.drawShapes = !b2d->dDraw.drawShapes;
}

void Physics::setVelocity(PhysicsComp *comp, const Vector2 &velocity)
{
    b2Vec2 vec { velocity.x, velocity.y };
    b2Body_SetLinearVelocity(comp->id, vec);
}

void Physics::setVelocityWithRotation(PhysicsComp *comp, const Vector2 &velocity)
{
    b2Vec2 vec { velocity.x, velocity.y };
    b2Vec2 norm = b2Normalize(vec);
    b2Rot rot { norm.x, norm.y };

    b2Body_SetLinearVelocity(comp->id, vec);
    auto pos = b2Body_GetPosition(comp->id);
    b2Body_SetTransform(comp->id, pos, rot);
}

void Physics::checkRectangleCollision(const Rectangle &rect, const std::function<void(GameObject *)> &callback)
{
    b2QueryFilter filter = b2DefaultQueryFilter();
    b2AABB aabb { rect.x,
        rect.y,
        rect.x + rect.width,
        rect.y + rect.height };
    OverlapResult res;
    b2World_OverlapAABB(b2d->worldId, aabb, filter, processOverlap, (void *)(&res));
    for (auto o : res.intersectedObjects) {
        callback(o);
    }
}

Vector2 Physics::getVelocity(PhysicsComp *comp)
{
    auto vel = b2Body_GetLinearVelocity(comp->id);
    Vector2 ret { vel.x, vel.y };
    return ret;
}

float Physics::getRotation(PhysicsComp *comp)
{
    auto r = b2Body_GetRotation(comp->id);
    return b2Rot_GetAngle(r);
}

void Physics::applyForce(PhysicsComp *comp, const Vector2 &dir)
{
    b2Vec2 vec { dir.x, dir.y };
    b2Body_ApplyForceToCenter(comp->id, vec, true);
}
