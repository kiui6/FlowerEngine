#include "OpaqueRenderPass.h"

#include <SDL3/SDL_gpu.h>

#include <Graphics/RenderElements/OpaqueSpriteRenderElement.h>
#include <Graphics/RenderEngine/RenderView/RenderObject.h>
#include <Graphics/RenderEngine/RenderUtils.h>

#include <Log/Log.h>
#include <Debug/Tracer/Tracer.h>
#include <Math/Mesh.h>

#include <generated/Shaders/OpaqueRenderPass/OpaqueSpriteQuad.vert.h>
#include <generated/Shaders/OpaqueRenderPass/OpaqueSpriteQuad.frag.h>

OpaqueRenderPass::OpaqueRenderPass(GPUContext& context, RenderStateStore& stateStore)
    : m_gpu(context), tilemapState(stateStore.Get<TilemapRenderState>()), globalState(stateStore.Get<GlobalRenderState>())
{
    for(auto& albedoTex : m_albedo) {
        SDL_GPUTextureCreateInfo texInfo = {};
        texInfo.type = SDL_GPU_TEXTURETYPE_2D;
        texInfo.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
        texInfo.width = globalState.canvasWidth;
        texInfo.height = globalState.canvasHeight;
        texInfo.layer_count_or_depth = 1;
        texInfo.num_levels = 1;
        texInfo.usage = SDL_GPU_TEXTUREUSAGE_COLOR_TARGET | SDL_GPU_TEXTUREUSAGE_SAMPLER;
        albedoTex = SDL_CreateGPUTexture(m_gpu.device, &texInfo);
        if(!albedoTex) { 
            LOG(Fatal, LogOpaqueRenderPass, "Failed to create Albedo Color Render Target");
            return;
        }
    }

    // Create Qaud Vertex Buffer
    m_quadVertexBuffer = RenderUtils::CreateBuffer(m_gpu.device, SDL_GPU_BUFFERUSAGE_VERTEX, (void*)Primitives::SQUARE, 6 * sizeof(Vertex2D));

    // Create Shaders
    m_opaqueSpritePipelineVertexShader = RenderUtils::CreateShader(
        m_gpu.device, 
        SDL_GPU_SHADERSTAGE_VERTEX, 
        OpaqueSpriteQuad_vert_h,
        OpaqueSpriteQuad_vert_h_size,
        0, 0, 2, 0);

    m_opaqueSpritePipelineFragmentShader = RenderUtils::CreateShader(
        m_gpu.device, 
        SDL_GPU_SHADERSTAGE_FRAGMENT, 
        OpaqueSpriteQuad_frag_h,
        OpaqueSpriteQuad_frag_h_size,
        1, 0, 0, 0);

    // Create Graphics Pipeline
    CreateOpaqueSpritePipeline();

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
    SDL_ReleaseGPUBuffer(m_gpu.device, m_quadVertexBuffer);

    for(const auto& albedoTex : m_albedo) {
        SDL_ReleaseGPUTexture(m_gpu.device, albedoTex);
    }

    SDL_ReleaseGPUSampler(m_gpu.device, m_opaqueSpriteSampler);

    SDL_ReleaseGPUShader(m_gpu.device, m_opaqueSpritePipelineVertexShader);
    SDL_ReleaseGPUShader(m_gpu.device, m_opaqueSpritePipelineFragmentShader);
    
    SDL_ReleaseGPUGraphicsPipeline(m_gpu.device, m_opaqueSpritePipeline);
}

void OpaqueRenderPass::CompileStaticObject(RenderResourceCompiler &resourceCompiler, const RenderObject &object)
{
}

void OpaqueRenderPass::CompileDynamicObject(RenderResourceCompiler &resourceCompiler, const RenderObject &object)
{
    for(const auto& opaqueSpriteElement : object.GetOpaqueSpriteElements()) {
        CompileOpaqueSpriteRenderElement(resourceCompiler, opaqueSpriteElement, m_dynamicOpaqueSpriteElements);
    }
}

void OpaqueRenderPass::PrepareFrame(SDL_GPUCommandBuffer* cmd, SDL_GPUCopyPass* copyPass)
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
            SDL_ReleaseGPUTransferBuffer(m_gpu.device, sprites.transferBuffer[m_gpu.currentFrame]);

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

void OpaqueRenderPass::RenderFrame(FrameContext &ctx)
{
    PUSH_TRACE_SCOPE("OpaqueRenderPass::Render()");
    ctx.imageAttachments[ImageRenderAttachment::Albedo] = m_albedo[ctx.frameIndex];

    // Can't render without world buffer
    if(ctx.bufferAttachments[BufferRenderAttachment::WorldData] == nullptr) {
        POP_TRACE_SCOPE();
        return;
    }

    BeginGPULabel(ctx.cmd, "Opaque");

    SDL_GPUColorTargetInfo colorTarget = {
        .texture     = m_albedo[ctx.frameIndex],
        .mip_level   = 0,
        .layer_or_depth_plane = 0,
        .clear_color = (SDL_FColor){ 0.88f, 0.722f, 0.23f, 1.0f },
        .load_op     = SDL_GPU_LOADOP_CLEAR,
        .store_op    = SDL_GPU_STOREOP_STORE,
    };

    SDL_GPURenderPass* pass = SDL_BeginGPURenderPass(ctx.cmd, &colorTarget, 1, NULL);

    // Render sprites
    BeginGPULabel(ctx.cmd, "Sprites");

    SDL_BindGPUGraphicsPipeline(pass, m_opaqueSpritePipeline);

    SDL_GPUBufferBinding vertBufferBindings{.buffer=m_quadVertexBuffer, .offset=0};
    SDL_BindGPUVertexBuffers(pass, 0, &vertBufferBindings, 1);

    SDL_BindGPUVertexStorageBuffers(pass, /*first slot*/ 0, &ctx.bufferAttachments[BufferRenderAttachment::WorldData], 1);

    for(auto& [atlasId, sprites] : m_dynamicOpaqueSpriteElements)
    {
        SDL_GPUTextureSamplerBinding atlasBinding = {
            .texture =  ctx.resources->texture2D[atlasId]->texture,
            .sampler = m_opaqueSpriteSampler
        };
        SDL_BindGPUFragmentSamplers(pass, /*first slot*/ 0, &atlasBinding, 1);

        SDL_BindGPUVertexStorageBuffers(pass, /*second slot*/ 1, &sprites.uniformBuffer[m_gpu.currentFrame], 1);

        SDL_DrawGPUPrimitives(pass, 6, sprites.assembly.size(), 0, 0);
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

void OpaqueRenderPass::CreateOpaqueSpritePipeline()
{
    SDL_GPUVertexBufferDescription vertexBindings[] = {{
        .slot       = 0,
        .pitch      = sizeof(Vertex2D),
        .input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX,
        .instance_step_rate = 0
    }};

    SDL_GPUVertexAttribute vertexAttribs[] = {
        {.location = 0, .buffer_slot = 0, .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2, .offset = offsetof(Vertex2D, pos)},
        {.location = 1, .buffer_slot = 0, .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2, .offset = offsetof(Vertex2D, uv)}
    };

    SDL_GPUVertexInputState vertexInput = {
        .vertex_buffer_descriptions = vertexBindings,
        .num_vertex_buffers = 1,
        .vertex_attributes = vertexAttribs,
        .num_vertex_attributes = 2
    };

    SDL_GPUGraphicsPipelineTargetInfo targetInfo = {};
    targetInfo.num_color_targets = 1;

    SDL_GPUColorTargetDescription colorTargetDesc = {};
    colorTargetDesc.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
    colorTargetDesc.blend_state = {
        .src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
        .dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
        .color_blend_op        = SDL_GPU_BLENDOP_ADD,
        .src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE,
        .dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ZERO,
        .alpha_blend_op        = SDL_GPU_BLENDOP_ADD,
        .color_write_mask      = SDL_GPU_COLORCOMPONENT_R | SDL_GPU_COLORCOMPONENT_G |
                                SDL_GPU_COLORCOMPONENT_B | SDL_GPU_COLORCOMPONENT_A,
        .enable_blend          = true,
    };

    targetInfo.color_target_descriptions = &colorTargetDesc;

    SDL_GPUGraphicsPipelineCreateInfo pipelineDesc = {
        .vertex_shader   = m_opaqueSpritePipelineVertexShader,
        .fragment_shader = m_opaqueSpritePipelineFragmentShader,
        .vertex_input_state = vertexInput,
        .primitive_type  = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,

        .rasterizer_state = {
            .fill_mode              = SDL_GPU_FILLMODE_FILL,
            .cull_mode              = SDL_GPU_CULLMODE_NONE,
            .front_face             = SDL_GPU_FRONTFACE_COUNTER_CLOCKWISE,
            .depth_bias_constant_factor    = 0.0f,
            .depth_bias_clamp       = 0.0f,
            .depth_bias_slope_factor = 0.0f,
            .enable_depth_clip      = true
        },

        .multisample_state = {
            .sample_count = SDL_GPU_SAMPLECOUNT_1,
            .sample_mask  = 0
        },

        .depth_stencil_state = {
            .compare_op         = SDL_GPU_COMPAREOP_ALWAYS,
            .back_stencil_state = {},
            .front_stencil_state= {},
            .enable_depth_test  = false,
            .enable_depth_write = false,
            .enable_stencil_test= false
        },


        .target_info = targetInfo,
    };

    m_opaqueSpritePipeline = SDL_CreateGPUGraphicsPipeline(m_gpu.device, &pipelineDesc);
    if (!m_opaqueSpritePipeline) {
        LOG(Fatal, LogOpaqueRenderPass, "Failed to create opaque sprite graphics pipeline");
    }
}

void OpaqueRenderPass::CompileOpaqueSpriteRenderElement(RenderResourceCompiler& resourceCompiler, const OpaqueSpriteRenderElement& element, std::unordered_map<uint64_t, OpaqueSpriteElementBatch>& container)
{
    if(element.texture == nullptr) {
        assert(!"Opaque Sprite Render Element's texture is nullptr");
        return;
    }

    OpaqueSpriteElementBatch* compiled;

    // Check if element already exists for the atlas texture
    auto it = container.find(element.texture->id);
    if(it != container.end()) {
        compiled = &it->second;
    } else {
        if(resourceCompiler.CompileTexture2D(*element.texture)) {
            assert(!"Error while compiling texture");
            return;
        }

        compiled = &container.emplace(element.texture->id, OpaqueSpriteElementBatch{}).first->second;
    }

    compiled->assembly.emplace_back(
        element.uv,
        element.tint,
        element.position,
        element.size,
        element.pivot,
        element.depth,
        element.rotation);
}
