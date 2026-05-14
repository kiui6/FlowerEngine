#pragma once

#include "RenderState.h"

#include <unordered_map>
#include <memory>

class RenderStateStore
{
    std::unordered_map<ID32, std::unique_ptr<RenderState>> m_states;

    template <RenderStateClass T>
    T* GetPointer() {
        auto it = m_states.find(T::StaticType());
        if(it != m_states.end()) {
            return static_cast<T*>(it->second.get());
        }

        T* state = new T();

        auto element = m_states.emplace(T::StaticType(), std::unique_ptr<T>(state));

        return static_cast<T*>(state);
    }
public:

    template <RenderStateClass T>
    const T& Get() {
        return *GetPointer<T>();
    }

    template <RenderStateClass T>
    T& GetMutable() {
        return *GetPointer<T>();
    }
};