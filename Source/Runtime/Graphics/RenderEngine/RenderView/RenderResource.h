#pragma once

#include <cstdint>
#include "RenderResourceType.h"

struct RenderResource {
    RenderResource(RenderResourceType initType) 
        : type(initType) {}

    uint64_t id = 0;
    bool isDirty = false;
protected:
    RenderResourceType type;
};

struct VertexBufferResource : public RenderResource {
    VertexBufferResource() : RenderResource(RenderResourceType::VertexBuffer) {}

    std::vector<Float4> vertices;
};

struct IndexBufferResource : public RenderResource {
    IndexBufferResource() : RenderResource(RenderResourceType::IndexBuffer) {}

    std::vector<uint32_t> indices;
};

struct UniformBufferResource : public RenderResource {
    UniformBufferResource() : RenderResource(RenderResourceType::UniformBuffer) {}
    std::vector<std::byte> data;
};

enum class Texture2DComponents : uint8_t {
    // Unsigned Integer Component
    A8_UINT, R8_UINT, R8G8_UINT, R8G8B8_UINT, R8G8B8A8_UINT,
    R16_UINT, R16G16_UINT, R16G16B16A16_UINT,
    R32_UINT, R32G32_UINT, R32G32B32A32_UINT,
    // Float Component
    A8_FLOAT, R8_FLOAT, R8G8_FLOAT, R8G8B8_FLOAT, R8G8B8A8_FLOAT,
    R16_FLOAT, R16G16_FLOAT, R16G16B16A16_FLOAT,
    R32_FLOAT, R32G32_FLOAT, R32G32B32A32_FLOAT,
    D32_FLOAT,
    // Unsigned Normalized Component
    A8_UNORM, R8_UNORM, R8G8_UNORM, R8G8B8_UNORM, R8G8B8A8_UNORM,
    R16_UNORM, R16G16_UNORM, R16G16B16A16_UNORM,
    R32_UNORM, R32G32_UNORM, R32G32B32A32_UNORM,
    D16_UNORM, D24_UNORM,
};

struct Texture2DResource : public RenderResource {
    Texture2DResource() : RenderResource(RenderResourceType::Texture2D) {}
    
    uint32_t width, height;
    Texture2DComponents components;
    // Byte data for the texture resource. Can be not set if object is not dirty
    std::vector<std::byte> data;
};