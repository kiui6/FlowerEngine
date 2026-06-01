#pragma once

#include <Graphics/RenderEngine/RenderState/RenderState.h>

class TilemapRenderState : public RenderState, public StaticallyTyped<MakeID32("TLMP")>
{

public:
    TilemapRenderState(GPUContext& gpu) : RenderState(gpu) {}

};