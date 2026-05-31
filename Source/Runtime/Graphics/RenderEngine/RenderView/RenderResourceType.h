#pragma once

#include <cstdint>

enum class RenderResourceType : uint16_t {
    VertexBuffer,
    IndexBuffer,
    UniformBuffer,
    StorageBuffer,
    Texture2D,
    Shader,
};