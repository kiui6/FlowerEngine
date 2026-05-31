#pragma once

#include <vector>
#include <cstdint>

#include <Graphics/RenderEngine/RenderState/RenderStateStore.h>

#include <Mixin/StaticallyTyped.h>

class SDL_GPUCommandBuffer;

struct RenderStateUpdateContext
{
    GPUContext& gpu;
    RenderStateStore& store;
    SDL_GPUCommandBuffer* cmd;
};

struct RenderStateUpdate
{
    RenderStateUpdate() = default;
    virtual ~RenderStateUpdate() = default;
    
    virtual void Apply(RenderStateUpdateContext& ctx) = 0;
};

template<typename T>
concept RenderStateUpdateClass = std::is_base_of_v<RenderStateUpdate, T> && requires {
    { T::StaticType() } -> std::same_as<ID32>;
};