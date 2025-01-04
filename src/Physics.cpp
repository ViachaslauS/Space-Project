#include "box2d/box2d.h"
#include "raylib.h"
#include "raymath.h"
#include "utils.h"

#include "Physics.hpp"

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
            TRACELOG(LOG_ERROR, "Zero vertices in shape passed to debug draw");
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
            TRACELOG(LOG_ERROR, "Zero vertices in shape passed to debug draw");
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
}

Physics::~Physics() = default;

Physics::Physics()
    : b2d(std::make_unique<B2d>())
{
    b2d->dDraw.DrawPolygon = &drawPolygon;
    b2d->dDraw.DrawSolidPolygon = &drawSolidPolygon;
    b2d->dDraw.DrawSolidCircle = &drawSolidCircle;

    float lengthUnitsPerMeter = 128.0f;
    b2SetLengthUnitsPerMeter(lengthUnitsPerMeter);

    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity.x = 0.0f;
    worldDef.gravity.y = 0.0f;

    b2d->worldId = b2CreateWorld(&worldDef);
}

PhysicsComp Physics::createRectangularBody(const Vector2 &pos, float width, float height)
{
    b2Vec2 extent = { width / 2.0f, height / 2.0f };
    b2Polygon poly = b2MakeBox(extent.x, extent.y);
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.position = { pos.x, pos.y };

    PhysicsComp comp;
    comp.id = b2CreateBody(b2d->worldId, &bodyDef);
    comps.push_back(comp);

    b2ShapeDef shapeDef = b2DefaultShapeDef();
    b2CreatePolygonShape(comp.id, &shapeDef, &poly);

    return comp;
}

PhysicsComp Physics::createCircularBody(const Vector2 &center, float radius)
{
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.position = { center.x, center.y };

    PhysicsComp comp;
    comp.id = b2CreateBody(b2d->worldId, &bodyDef);
    comps.push_back(comp);

    b2Vec2 c { 0.0f, 0.0f };
    b2Circle circle { c, radius };
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    b2CreateCircleShape(comp.id, &shapeDef, &circle);

    return comp;
}

bool Physics::removeBody(const PhysicsComp &comp)
{
    size_t i = 0;
    while (i < comps.size())
    {
        if (B2_ID_EQUALS(comps[i].id, comp.id))
        {
            b2DestroyBody(comp.id);
            comps[i] = comps.back();
            comps.pop_back();
            return true;
        }
        i++;
    }

    // no such comp >:(
    TRACELOG(LOG_ERROR, "Failed to find PhysicsComp");
    return false;
}

void Physics::update()
{
    b2World_Step(b2d->worldId, 0.016f, 4);
}

void Physics::debugRender()
{
    b2World_Draw(b2d->worldId, &b2d->dDraw);
}

void Physics::toggleDebugRender()
{
    b2d->dDraw.drawShapes = !b2d->dDraw.drawShapes;
}
