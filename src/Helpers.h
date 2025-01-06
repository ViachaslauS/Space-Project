#pragma once

#include <map>
#include <functional>
#include <random>
#include <cmath>
#include <string>

#include "raylib.h"

namespace helpers
{
    Vector2 windowCenter();
    float vecToAngle(const Vector2 &vec);

    Vector2 getDrawPosInRectCenter(Rectangle rect, std::string text, float fontSize);

    using DelegateHandle = uint32_t;

    template <class... Args>
    class MulticastDelegate
    {
    public:
        DelegateHandle add(std::function<void(Args...)>&& item)
        {
            m_delegates[++m_idCounter] = item;
            return m_idCounter;
        }

        void broadcast(Args... args)
        {
            for (auto&& [id, delegateFunc] : m_delegates)
            {
                delegateFunc(args...);
            }
        }

        bool isValid(DelegateHandle handle) const
        {
            return m_delegates.find(handle) != m_delegates.end();
        }

        void empty()
        {
            m_delegates.empty();
        }

        void clear()
        {
            m_delegates.clear();
        }

        void remove(DelegateHandle handle)
        {
            m_delegates.erase(handle);
        }

    private:
        uint32_t m_idCounter = 0u;

        std::map<DelegateHandle, std::function<void(Args...)>> m_delegates;
    };


    inline float randFlt()
    {
        return static_cast<float>(std::rand()) / RAND_MAX;
    }

    inline float randFlt(float min, float max)
    {
        return min + (static_cast<float>(std::rand()) / RAND_MAX) * (max - min);
    }

    template <class T>
    T lerpWithDeviation(T base, T deviation, float progress)
    {
        return std::lerp(base - deviation, base + deviation, progress);
    }

    static bool isInWindow(Vector2 pos)
    {
        return pos.x < 1920 && pos.x > 0 && pos.y < 1080 && pos.y > 0;
    }

    inline float lerpTudaSuda(float t)
    {
        return std::abs(t - (std::floor(t * 2))) * 2;
    }

    template <class ArrType>
    bool isValidIdx(const ArrType& arr, auto idx)
    {
        return idx >= 0 && idx < arr.size();
    }
}
