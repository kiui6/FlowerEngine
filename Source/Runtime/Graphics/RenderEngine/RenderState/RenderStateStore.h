#pragma once

#include "RenderState.h"

#include <unordered_map>
#include <memory>

class RenderStateStore
{
    GPUContext& m_gpu;

    mutable std::unordered_map<ID32, std::unique_ptr<RenderState>> m_states;

    template <RenderStateClass T>
    T* GetOrCreate() const {
        auto it = m_states.find(T::StaticType());
        if(it != m_states.end()) {
            return static_cast<T*>(it->second.get());
        }

        T* state = new T(m_gpu);

        auto element = m_states.emplace(T::StaticType(), std::unique_ptr<T>(state));

        return static_cast<T*>(state);
    }
public:

    RenderStateStore() = delete;
    RenderStateStore(GPUContext& gpu) : m_gpu(gpu) {}

    template <RenderStateClass T>
    const T& Get() const {
        return *GetOrCreate<T>();
    }

    template <RenderStateClass T>
    T& GetMutable() {
        return *GetOrCreate<T>();
    }
};