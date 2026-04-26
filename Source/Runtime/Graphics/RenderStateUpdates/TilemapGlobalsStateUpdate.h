#pragma once

#include <Graphics/RenderEngine/RenderView/RenderStateUpdate.h>

struct TilemapGlobalsStateUpdate : public RenderStateUpdate
{
    TilemapGlobalsStateUpdate() : RenderStateUpdate(RenderStateUpdateType::TilemapGlobals) {}
};