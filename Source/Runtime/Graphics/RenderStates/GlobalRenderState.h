#pragma once

#include <Graphics/RenderEngine/RenderState/RenderState.h>

#include <Math/Matrix.h>
#include <Math/Vectors.h>

struct alignas(16) GPUWorldData {
    Float4x4 projectionView;
    Float3 position;
    uint8_t pad1;
    uint8_t pad2;
    uint8_t pad3;
    uint8_t pad4;
};

static_assert(sizeof(GPUWorldData) == 80, "WorldBuffer must be 80 bytes");
static_assert(offsetof(GPUWorldData, projectionView) == 0,  "projectionView offset wrong");
static_assert(offsetof(GPUWorldData, position) == 64,       "position offset wrong");

struct GlobalRenderState : public RenderState
{
    GlobalRenderState(GPUContext& gpu);

    static ID32 StaticType() {return MakeID32("GLOB");} 

    Float4x4 projectionMatrix;
    uint32_t canvasWidth, canvasHeight;
    Float3 cameraPosition;

    SDL_GPUBuffer* worldBuffer = nullptr;
};