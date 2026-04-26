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

enum class Texture2DComponents : uint8_t {
    R8, R8G8, R8G8B8, R8G8B8A8,
    R16, R16G16, R16G16B16,
    R32, R32G32, R32G32B32,
};

struct Texture2DResource : public RenderResource {
    uint32_t width, height;
    Texture2DComponents components;
    // Byte data for the texture resource. Can be not set if object is not dirty
    std::vector<std::byte> data;
};