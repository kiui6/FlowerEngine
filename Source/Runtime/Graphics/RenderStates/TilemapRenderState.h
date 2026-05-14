#pragma once

#include <Graphics/RenderEngine/RenderState/RenderState.h>

class TilemapRenderState : public RenderState
{

public:
    static ID32 StaticType() {return MakeID32("TLMP");} 
};