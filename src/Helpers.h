// by Slava

#pragma once

#include <map>
#include <functional>

namespace helpers
{
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
            m_idCounter = 0u;
            m_delegates.empty();
        }

        void remove(DelegateHandle handle)
        {
            m_delegates.erase(handle);
        }

    private:
        uint32_t m_idCounter = 0u;

        std::map<DelegateHandle, std::function<void(Args...)>> m_delegates;
    };
}