#pragma once

#include <Graphics/RenderEngine/RenderView/RenderResource.h>
#include <Math/Vectors.h>
#include <vector>

#include <SDL3/SDL_gpu.h>

#include <Graphics/RenderEngine/RenderUtils.h>

struct AtlasTilemapChunkLayerRenderElement
{
    struct TileElement {
        // minU, minV, maxU, maxV
        Float4 uv = {0, 0, 1, 1};
        Int2 position = {0, 0};
    };

    std::vector<TileElement> tiles;
    Int2 position = {0, 0};
    Texture2DResource* atlasTexture = nullptr;
    int32_t depth = 0;

};