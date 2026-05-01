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
    R8, R8G8, R8G8B8, R8G8B8A8,
    R16, R16G16, R16G16B16,
    R32, R32G32, R32G32B32,
};

struct Texture2DResource : public RenderResource {
    Texture2DResource() : RenderResource(RenderResourceType::Texture2D) {}
    
    uint32_t width, height;
    Texture2DComponents components;
    // Byte data for the texture resource. Can be not set if object is not dirty
    std::vector<std::byte> data;
};