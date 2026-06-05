#include "UpscaleRenderPass.h"

#include <Math/Mesh.h>
#include <Log/Log.h>
#include <Graphics/RenderEngine/RenderUtils.h>

// Shaders
#include "Shaders/FullscreenTriangle.vert.h"
#include "Shaders/FullscreenTriangle.frag.h"

UpscaleRenderPass::UpscaleRenderPass(GPUContext &context, RenderStateStore& stateStore)
    : m_gpu(context), m_state(stateStore.Get<UpscaleRenderState>())
{
    m_state.SubscribeToChange(this, &UpscaleRenderPass::OnStateChange);

    m_upscaleBuffer = RenderUtils::CreateBuffer(context.device, SDL_GPU_BUFFERUSAGE_GRAPHICS_STORAGE_READ, (void*)&m_state.compensationCalculation, sizeof(m_state.compensationCalculation));

    m_upscaleTransferBuffer = RenderUtils::CreateTransferBuffer(context.device, sizeof(m_state.compensationCalculation));

    m_vertexShader = RenderUtils::CreateShader(
        m_gpu.device, 
        SDL_GPU_SHADERSTAGE_VERTEX, 
        FullscreenTriangle_vert_h,
        sizeof(FullscreenTriangle_vert_h),
        0, 0, 1, 0);

    m_fragmentShader = RenderUtils::CreateShader(
        m_gpu.device, 
        SDL_GPU_SHADERSTAGE_FRAGMENT, 
        FullscreenTriangle_frag_h,
        sizeof(FullscreenTriangle_frag_h),
        1, 0, 0, 0);

    CreateRenderPipeline();

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
    m_sampler = SDL_CreateGPUSampler(m_gpu.device, &samplerInfo);
    if(!m_sampler) { 
        LOG(Fatal, LogUpscaleRenderPass, "Failed to create Upscale Sampler");
        return;
    }
}

UpscaleRenderPass::~UpscaleRenderPass()
{
    SDL_ReleaseGPUBuffer(m_gpu.device, m_upscaleBuffer);
}

void UpscaleRenderPass::PrepareFrame(SDL_GPUCommandBuffer *cmd, SDL_GPUCopyPass *copyPass)
{
    if(m_upscaleBufferDirty) {
        SDL_WaitForGPUIdle(m_gpu.device);

        RenderUtils::UpdateBufferWithTransferBufferAndPass(m_gpu.device, cmd, copyPass, 
                                        m_upscaleTransferBuffer, m_upscaleBuffer, 
                                        (void*)(&m_state.compensationCalculation), sizeof(m_state.compensationCalculation));

        m_upscaleBufferDirty = false;
    }
}

void UpscaleRenderPass::RenderFrame(FrameContext &ctx)
{
    BeginGPULabel(ctx.cmd, "Upscale");

    SDL_GPUColorTargetInfo colorTarget = {
        .texture     = ctx.swapchainTexture,
        .mip_level   = 0,
        .layer_or_depth_plane = 0,
        .clear_color = (SDL_FColor){ 0.88f, 0.722f, 0.23f, 1.0f },
        .load_op     = SDL_GPU_LOADOP_CLEAR,
        .store_op    = SDL_GPU_STOREOP_STORE,
    };

    SDL_GPURenderPass* pass = SDL_BeginGPURenderPass(ctx.cmd, &colorTarget, 1, NULL);

    SDL_BindGPUGraphicsPipeline(pass, m_pipeline);

    SDL_GPUTextureSamplerBinding atlasBinding = {
            // TODO: Change to Final once Lightning pass is operational
            .texture =  ctx.imageAttachments[ImageRenderAttachment::Albedo],
            .sampler = m_sampler
        };
        
    SDL_BindGPUFragmentSamplers(pass, /*first slot*/ 0, &atlasBinding, 1);

    SDL_BindGPUVertexStorageBuffers(pass, /*second slot*/ 0, &m_upscaleBuffer, 1);

    SDL_DrawGPUPrimitives(pass, 3, 1, 0, 0);

    SDL_EndGPURenderPass(pass);

    EndGPULabel(ctx.cmd);
}

void UpscaleRenderPass::CreateRenderPipeline()
{
    SDL_GPUVertexInputState vertexInput = {
        .vertex_buffer_descriptions = nullptr,
        .num_vertex_buffers = 0,
        .vertex_attributes = nullptr,
        .num_vertex_attributes = 0
    };

    SDL_GPUGraphicsPipelineTargetInfo targetInfo = {};
    targetInfo.num_color_targets = 1;

    SDL_GPUColorTargetDescription colorTargetDesc = {};
    colorTargetDesc.format = m_gpu.swapchainFormat;
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
        .vertex_shader   = m_vertexShader,
        .fragment_shader = m_fragmentShader,
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

    m_pipeline = SDL_CreateGPUGraphicsPipeline(m_gpu.device, &pipelineDesc);
    if (!m_pipeline) {
        LOG(Fatal, LogUpscaleRenderPass, "Failed to create upscale graphics pipeline");
    }
}

void UpscaleRenderPass::OnStateChange()
{
    m_upscaleBufferDirty = true;
}
