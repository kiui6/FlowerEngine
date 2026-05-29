#include "OpaqueRenderPass.h"

#include <SDL3/SDL_gpu.h>

#include <Graphics/RenderElements/OpaqueSpriteRenderElement.h>
#include <Graphics/RenderEngine/RenderUtils.h>

#include <Log/Log.h>
#include <Debug/Tracer/Tracer.h>

OpaqueRenderPass::OpaqueRenderPass(GPUContext& context, RenderStateStore& stateStore)
    : m_gpu(context), tilemapState(stateStore.Get<TilemapRenderState>()), globalState(stateStore.Get<GlobalRenderState>())
{
    SDL_GPUTextureCreateInfo texInfo = {};
    texInfo.type = SDL_GPU_TEXTURETYPE_2D;
    texInfo.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
    texInfo.width = globalState.canvasWidth;
    texInfo.height = globalState.canvasHeight;
    texInfo.layer_count_or_depth = 1;
    texInfo.num_levels = 1;
    texInfo.usage = SDL_GPU_TEXTUREUSAGE_COLOR_TARGET | SDL_GPU_TEXTUREUSAGE_SAMPLER;
    m_albedo = SDL_CreateGPUTexture(m_gpu.device, &texInfo);
    if(!m_albedo) { 
        LOG(Fatal, LogOpaqueRenderPass, "Failed to create Albedo Color Render Target");
        return;
    }

    // Create Sampler
    SDL_GPUSamplerCreateInfo samplerInfo = {};
    samplerInfo.min_filter = SDL_GPU_FILTER_NEAREST;
    samplerInfo.mag_filter = SDL_GPU_FILTER_NEAREST;
    samplerInfo.mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_NEAREST;
    samplerInfo.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
    samplerInfo.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
    samplerInfo.address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
    samplerInfo.mip_lod_bias = 0.0f;
    samplerInfo.min_lod = 0.0f;
    samplerInfo.max_lod = 0.0f;  
    samplerInfo.enable_anisotropy = false;
    samplerInfo.max_anisotropy = 1;
    samplerInfo.compare_op = SDL_GPU_COMPAREOP_ALWAYS;
    m_opaqueSpriteSampler = SDL_CreateGPUSampler(m_gpu.device, &samplerInfo);
    if(!m_opaqueSpriteSampler) { 
        LOG(Fatal, LogOpaqueRenderPass, "Failed to create Opaque Sprite Sampler");
        return;
    }
}

OpaqueRenderPass::~OpaqueRenderPass()
{
    SDL_ReleaseGPUTexture(m_gpu.device, m_albedo);
}

void OpaqueRenderPass::Compile(RenderResourceCompiler &resourceCompiler, RenderObject *object, RenderElement *element)
{
    switch(element->GetRenderElementType()) {
        case RenderElementType::Sprite:
            CompileOpaqueSpriteRenderElement(resourceCompiler, object, static_cast<OpaqueSpriteRenderElement*>(element));
            break;
        default:
            assert(!"Unsupported render element.");
            return;
    }
}

void OpaqueRenderPass::Prepare(SDL_GPUCommandBuffer* cmd, SDL_GPUCopyPass* copyPass)
{
    BeginGPULabel(cmd, "Opaque");

    BeginGPULabel(cmd, "Static Sprites");
    for(auto& [atlasId, sprites] : m_staticOpaqueSpriteElements) {
        if(sprites.bIsDirty) {

        }
    }
    EndGPULabel(cmd); // Static Sprites

    BeginGPULabel(cmd, "Dynamic Sprites");
    for(auto& [atlasId, sprites] : m_dynamicOpaqueSpriteElements) {
        // Determine if we need to grow uniform buffer for opaque sprite elements
        if(sprites.bufferSize[m_gpu.currentFrame] < (sprites.assembly.size() * sprites.elementSize)) {
            // Recreate buffer
            SDL_ReleaseGPUBuffer(m_gpu.device, sprites.uniformBuffer[m_gpu.currentFrame]);

            sprites.uniformBuffer[m_gpu.currentFrame] = RenderUtils::CreateBuffer(m_gpu.device, SDL_GPU_BUFFERUSAGE_GRAPHICS_STORAGE_READ, nullptr, sprites.assembly.size() * sprites.elementSize);
            sprites.bufferSize[m_gpu.currentFrame] = sprites.assembly.size() * sprites.elementSize;

            SDL_GPUTransferBufferCreateInfo tbInfo = {};
            tbInfo.size = sprites.assembly.size() * sprites.elementSize;
            tbInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
            sprites.transferBuffer[m_gpu.currentFrame] = SDL_CreateGPUTransferBuffer(m_gpu.device, &tbInfo);
        }

        // Upload new buffer data
        // TODO: Dirty flag

        void* mapped = SDL_MapGPUTransferBuffer(m_gpu.device, sprites.transferBuffer[m_gpu.currentFrame], false);
        memcpy(mapped, sprites.assembly.data(), sprites.assembly.size() * sprites.elementSize);
        SDL_UnmapGPUTransferBuffer(m_gpu.device, sprites.transferBuffer[m_gpu.currentFrame]);

        const SDL_GPUTransferBufferLocation tbBufferLocation = {
            .transfer_buffer = sprites.transferBuffer[m_gpu.currentFrame],
            .offset = 0
        };
        const SDL_GPUBufferRegion vBufferLocation = {
            .buffer = sprites.uniformBuffer[m_gpu.currentFrame],
            .offset = 0,
            .size = (uint32_t)sprites.assembly.size() * (uint32_t)sprites.elementSize
        };

        SDL_UploadToGPUBuffer(copyPass, &tbBufferLocation, &vBufferLocation, false);
    }
    EndGPULabel(cmd); // Dynamic Sprites

    EndGPULabel(cmd); // Opaque
}

void OpaqueRenderPass::Render(FrameContext &ctx)
{
    PUSH_TRACE_SCOPE("OpaqueRenderPass::Render()");
    ctx.attachments[(uint8_t)RenderAttachment::Albedo] = m_albedo;

    BeginGPULabel(ctx.cmd, "Opaque");

    SDL_GPUColorTargetInfo colorTarget = {
        .texture     = m_albedo,
        .mip_level   = 0,
        .layer_or_depth_plane = 0,
        .clear_color = (SDL_FColor){ 0.88f, 0.722f, 0.23f, 1.0f },
        .load_op     = SDL_GPU_LOADOP_CLEAR,
        .store_op    = SDL_GPU_STOREOP_STORE,
    };

    SDL_GPURenderPass* pass = SDL_BeginGPURenderPass(ctx.cmd, &colorTarget, 1, NULL);

    // Render sprites
    BeginGPULabel(ctx.cmd, "Sprites");

    for(auto& [atlasId, sprites] : m_dynamicOpaqueSpriteElements)
    {
        SDL_GPUTextureSamplerBinding atlasBinding = {
            .texture =  ctx.resources->texture2D[atlasId]->texture,
            .sampler = m_opaqueSpriteSampler
        };
        SDL_BindGPUFragmentSamplers(pass, /*first slot*/ 0, &atlasBinding, 1);

        SDL_BindGPUVertexStorageBuffers(pass, /*second slot*/ 1, &sprites.uniformBuffer[m_gpu.currentFrame], 1);
    }

    EndGPULabel(ctx.cmd);

    SDL_EndGPURenderPass(pass);

    EndGPULabel(ctx.cmd);
    
    POP_TRACE_SCOPE();
}

void OpaqueRenderPass::Cleanup()
{
    for(auto& [atlasId, sprites] : m_dynamicOpaqueSpriteElements) {
        sprites.assembly.clear();
    }
}

void OpaqueRenderPass::CompileOpaqueSpriteRenderElement(RenderResourceCompiler &resourceCompiler, RenderObject *object, OpaqueSpriteRenderElement *element)
{
    OpaqueSpriteElementBatch* compiled;

    // Check if element already exists for the atlas texture
    auto it = m_dynamicOpaqueSpriteElements.find(element->texture->id);
    if(it != m_dynamicOpaqueSpriteElements.end()) {
        compiled = &it->second;
    } else {
        if(!element->texture) {
            assert(!"element->texture == nullptr");
            return;
        }
    
        resourceCompiler.CompileTexture2D(*element->texture);

        compiled = &m_dynamicOpaqueSpriteElements.emplace(element->texture->id, OpaqueSpriteElementBatch{}).first->second;
    }

    compiled->assembly.emplace_back(
        element->position,
        element->depth,
        element->scale,
        element->rotation,
        element->tint,
        element->uv);
}
