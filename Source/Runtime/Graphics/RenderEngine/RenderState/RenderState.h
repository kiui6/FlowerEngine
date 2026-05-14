#pragma once

#include <Mixin/Typed.h>

#include <concepts>

struct RenderState
{
    static ID32 StaticType() {return MakeID32("NONE");} 
};

template<typename T>
concept RenderStateClass = std::is_base_of<RenderState, T>::value;