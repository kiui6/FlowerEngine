#pragma once

#include <Mixin/Typed.h>
#include <Graphics/RenderEngine/GPUContext.h>

#include <Delegate/Delegate.h>
#include <Mixin/StaticallyTyped.h>

#include <concepts>

struct RenderState
{
    RenderState() = delete;
    RenderState(GPUContext& gpu) : m_gpu(gpu) {}
    virtual ~RenderState() = default;

    inline void NotifyChanged() {m_onChangeDelegate.Broadcast();}

    template <typename UserClass>
    inline void SubscribeToChange(UserClass* user, MulticastDelegate<>::MemFuncType<UserClass> func) const {m_onChangeDelegate.BindMember(user, func);}
protected:
    GPUContext& m_gpu;
    mutable MulticastDelegate<> m_onChangeDelegate;
};

template<typename T>
concept RenderStateClass = std::is_base_of_v<RenderState, T> && requires {
    { T::StaticType() } -> std::same_as<ID32>;
};