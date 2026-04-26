#pragma once

#include <cstdint>

struct RenderResource {
    uint64_t id = 0;
    bool isDirty = false;
};

struct VertexBufferResource : public RenderResource {
    std::vector<Float4> vertices;
};

struct IndexBufferResource : public RenderResource {
    std::vector<uint32_t> indices;
};

struct UniformBufferResource : public RenderResource {
    std::vector<std::byte> data;
};

struct Texture2DBufferResource : public RenderResource {
    
};