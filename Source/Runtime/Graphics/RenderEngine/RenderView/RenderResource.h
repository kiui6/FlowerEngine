#pragma once

#include <cstdint>

enum class RenderResourceType : uint16_t {
    VertexBuffer,
    IndexBuffer,
    UniformBuffer,
    StorageBuffer,
    Texture2D,
    Sampler2D
};

struct RenderResource {
    uint64_t id = 0;

    RenderResourceType type;

    size_t dataSize = 0;
    void* data = nullptr;

    bool isDirty = false;
};