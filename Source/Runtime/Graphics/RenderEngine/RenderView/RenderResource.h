#pragma once

#include <cstdint>
#include <memory>
#include "RenderResourceType.h"

#include "CompiledRenderResource.h"

#include <Math/Vectors.h>

struct RenderResource {
    RenderResource(RenderResourceType initType) 
        : type(initType) {}

    uint64_t id = 0;
    bool isDirty = false;

    std::weak_ptr<CompiledRenderResource>& GetCompiledResource() { return m_compiledResource; }
protected:
    RenderResourceType type;
    std::weak_ptr<CompiledRenderResource> m_compiledResource;

    friend class RenderResourceCompiler;
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

struct Texture2DResource : public RenderResource {
    Texture2DResource() : RenderResource(RenderResourceType::Texture2D) {}
    
    uint32_t width, height;
    Texture2DComponents components;
    // Byte data for the texture resource. Can be not set if object is not dirty
    unsigned char* data;
    size_t dataSize;
};

struct ShaderResource : public RenderResource {
    ShaderResource() : RenderResource(RenderResourceType::Shader) {}
    
    // Byte data for SPIRV. Can be not set if object is not dirty
    unsigned char* data;
    size_t dataSize;
};