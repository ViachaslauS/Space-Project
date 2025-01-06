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

    Vector2 getDrawPosInRectCenter(const Rectangle &rect, const std::string &text, float fontSize)
    {
        const float xCenter = text.size() * fontSize * 0.5f;

        Vector2 outPos;

        outPos.x = rect.x + rect.width * 0.5f - xCenter * 0.5f;
        outPos.y = rect.y + rect.height * 0.5f - fontSize * 0.5f;

        return outPos;
    }
}
