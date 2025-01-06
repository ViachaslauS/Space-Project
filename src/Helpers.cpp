#include "Helpers.h"

#include "box2d/box2d.h"

namespace helpers
{
    Vector2 windowCenter()
    {
        return { GetScreenWidth() * 0.5f, GetScreenHeight() * 0.5f };
    }

    float vecToAngle(const Vector2 &vec)
    {
        b2Vec2 v { vec.x, vec.y };
        b2Vec2 norm = b2Normalize(v);
        b2Rot rot { norm.x, norm.y };
        return b2Rot_GetAngle(rot);
    }
}
