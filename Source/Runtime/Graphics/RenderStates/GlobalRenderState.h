#pragma once

#include <Graphics/RenderEngine/RenderState/RenderState.h>
#include <Graphics/RenderEngine/RenderConstants.h>

#include <Math/Matrix.h>
#include <Math/Vectors.h>

#include <array>

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

struct GlobalRenderState : public RenderState, public StaticallyTyped<MakeID32("GLOB")>
{
    GlobalRenderState(GPUContext& gpu);
    ~GlobalRenderState();

    Float4x4 projectionMatrix;
    uint32_t canvasWidth, canvasHeight;
    Float3 cameraPosition;

    std::array<SDL_GPUBuffer*, FRAMES_IN_FLIGHT> worldBuffer;
};