#pragma once

#include <Math/Matrix.h>
#include <Math/Vectors.h>

struct alignas(16) GPUWorldBufferData {
    Float4x4 projectionView;
    Float3 position;
    uint8_t pad1;
    uint8_t pad2;
    uint8_t pad3;
    uint8_t pad4;
};

static_assert(sizeof(GPUWorldBufferData) == 80, "WorldBuffer must be 80 bytes");
static_assert(offsetof(GPUWorldBufferData, projectionView) == 0,  "projectionView offset wrong");
static_assert(offsetof(GPUWorldBufferData, position) == 64,       "position offset wrong");