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
        // TODO: proper debug drawing, this doesn't work
        drawPolygon(vertices, vertexCount, color, context);
    }
}

Physics::~Physics() = default;

Physics::Physics()
    : b2d(std::make_unique<B2d>())
{
    b2d->dDraw.DrawPolygon = &drawPolygon;
    b2d->dDraw.DrawSolidPolygon = &drawSolidPolygon;

    float lengthUnitsPerMeter = 128.0f;
    b2SetLengthUnitsPerMeter(lengthUnitsPerMeter);

    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity.x = 0.0f;
    worldDef.gravity.y = 0.0f;

    b2d->worldId = b2CreateWorld(&worldDef);
}

PhysicsComp Physics::createRectangularBody(const Vector2 &pos, float width, float height)
{
    b2Vec2 extent = { width, height };
    b2Polygon poly = b2MakeBox(extent.x, extent.y);
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.position = { pos.x - width / 2, pos.y - height / 2};

    PhysicsComp comp;
    comp.id = b2CreateBody(b2d->worldId, &bodyDef);
    comps.push_back(comp);

    b2ShapeDef shapeDef = b2DefaultShapeDef();
    b2CreatePolygonShape(comp.id, &shapeDef, &poly);

    return comp;
}

bool Physics::removeBody(const PhysicsComp &comp)
{
    size_t i = 0z;
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
    TRACELOG(LOG_ERROR, "Failed to find PhysicsComp");
    // no such comp >:(
    return false;
}

void Physics::update()
{
    b2World_Step(b2d->worldId, 0.016f, 5);
}

void Physics::debugRender()
{
    b2World_Draw(b2d->worldId, &b2d->dDraw);
}

void Physics::toggleDebugRender()
{
    b2d->dDraw.drawShapes = !b2d->dDraw.drawShapes;
}
