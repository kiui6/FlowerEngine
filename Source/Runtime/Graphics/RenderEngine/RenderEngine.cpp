#include "RenderEngine.h"

#include <Log/Log.h>

#include <Graphics/RenderPasses/OpaqueRenderPass/OpaqueRenderPass.h>
#include <Graphics/RenderPasses/ReliefRenderPass.h>
#include <Graphics/RenderPasses/LightingRenderPass.h>
#include <Graphics/RenderPasses/UpscaleRenderPass/UpscaleRenderPass.h>
#include <Graphics/RenderPasses/DebugUIRenderPass/DebugUIRenderPass.h>

#include "RenderUtils.h"

#include <Debug/Tracer/Tracer.h>

RenderEngine::RenderEngine()
    : m_stateStore(m_ctx)
{
}

void RenderEngine::Initialize(SDL_Window* window)
{
    m_ctx.window = window;
    if (!m_ctx.window) {
        LOG(Fatal, LogRender, "No window provided!");
        return;
    }

    // Create devoce
    {
        SDL_PropertiesID props = SDL_CreateProperties();
        SDL_SetStringProperty(props, SDL_PROP_GPU_DEVICE_CREATE_NAME_STRING, NULL);
        SDL_SetBooleanProperty(props, SDL_PROP_GPU_DEVICE_CREATE_SHADERS_SPIRV_BOOLEAN, true);
        
        #ifndef NDEBUG
        SDL_SetBooleanProperty(props, SDL_PROP_GPU_DEVICE_CREATE_DEBUGMODE_BOOLEAN, true);
        #else
        SDL_SetBooleanProperty(props, SDL_PROP_GPU_DEVICE_CREATE_DEBUGMODE_BOOLEAN, false);
        #endif

        m_ctx.device = SDL_CreateGPUDeviceWithProperties(props);
        SDL_DestroyProperties(props);

        if (!m_ctx.device) {
            LOGF(Fatal, LogRender, "GPU device creation failed: %s", SDL_GetError());
            return;
        }
    }

    // Claim window for GPU rendering
    if (!SDL_ClaimWindowForGPUDevice(m_ctx.device, m_ctx.window)) {
        LOGF(Fatal, LogRender, "Window GPU claiming failed: %s", SDL_GetError());
        return;
    }

    SDL_SetGPUSwapchainParameters(m_ctx.device, m_ctx.window, SDL_GPU_SWAPCHAINCOMPOSITION_SDR, SDL_GPU_PRESENTMODE_VSYNC);
    
    m_ctx.swapchainFormat = SDL_GetGPUSwapchainTextureFormat(m_ctx.device, m_ctx.window);

    // Initialize Render Passes
    m_renderPasses[(uint32_t)RenderPassType::Opaque] = std::make_unique<OpaqueRenderPass>(m_ctx, m_stateStore);
    m_renderPasses[(uint32_t)RenderPassType::Relief] = std::make_unique<ReliefRenderPass>(m_ctx);
    m_renderPasses[(uint32_t)RenderPassType::Lighting] = std::make_unique<LightingRenderPass>(m_ctx);
    m_renderPasses[(uint32_t)RenderPassType::Upscale] = std::make_unique<UpscaleRenderPass>(m_ctx, m_stateStore);
    m_renderPasses[(uint32_t)RenderPassType::DebugUI] = std::make_unique<DebugUIRenderPass>(m_ctx, m_stateStore);

    LOG(Log, LogRender, "Render Engine initialization complete");
}

void RenderEngine::Render(float deltaTime, RenderView &renderView)
{
    PUSH_TRACE_SCOPE("RenderEngine::Render()");
    // Execute Render Jobs
    PUSH_TRACE_SCOPE("Execute Jobs");
    for(const auto& job : renderView.m_renderJobs) {
        job->Execute();
    }
    POP_TRACE_SCOPE();

    // Compile elements
    PUSH_TRACE_SCOPE("Compile Elements");
    RenderResourceCompiler resourceCompiler(m_ctx, m_compiledRes);
    if(renderView.m_staticRenderObjectsDirty) {
        for(const auto& [id, rendObject] : renderView.m_staticRenderObjects) {
            if(rendObject.lastReferencedFramesAgo != 0) continue;

            for(const auto& rendElement : rendObject.object->GetElements()) {
                m_renderPasses[(uint32_t)rendElement->GetRenderPassType()]->Compile(resourceCompiler, rendObject.object.get(), rendElement.get());
            }
        }
    }

    for(const auto& [id, rendObject] : renderView.m_dynamicRenderObjects) {
        for(const auto& rendElement : rendObject->GetElements()) {
            m_renderPasses[(uint32_t)rendElement->GetRenderPassType()]->Compile(resourceCompiler, rendObject.get(), rendElement.get());
        }
    }
    POP_TRACE_SCOPE();

    // Start recording main command buffer
    SDL_GPUCommandBuffer *cmd = SDL_AcquireGPUCommandBuffer(m_ctx.device);

    // Perform State Updates
    PUSH_TRACE_SCOPE("State Update");
    BeginGPULabel(cmd, "State Update");
    RenderStateUpdateContext renderStateUpdateContext {
        .gpu = m_ctx,
        .store = m_stateStore,
        .cmd = cmd
    };
    for(auto& [id, updateObj] : renderView.m_stateUpdates) {
        if(updateObj.isReferenced) {
            updateObj.object->Apply(renderStateUpdateContext);
            updateObj.isReferenced = false;
        }
    }
    EndGPULabel(cmd);
    POP_TRACE_SCOPE();

    // After all data is assembled into batches, it can be compiled into GPU resources
    PUSH_TRACE_SCOPE("Prepare Frame");
    BeginGPULabel(cmd, "Prepare Frame");
    SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(cmd);

    for(const auto& pass : m_renderPasses) {
        pass->Prepare(cmd, copyPass);
    }

    SDL_EndGPUCopyPass(copyPass);
    EndGPULabel(cmd);
    POP_TRACE_SCOPE();

    SDL_GPUTexture *swapchainTexture = NULL;
    uint32_t swapchainTextureW, swapchainTextureH;
    SDL_AcquireGPUSwapchainTexture(cmd, m_ctx.window, &swapchainTexture, &swapchainTextureW, &swapchainTextureH);
    // Don't attempt any rendering if swapchain texture is invalid, it will crash
    if(!swapchainTexture) {
        SDL_SubmitGPUCommandBuffer(cmd);
        SDL_WaitForGPUSwapchain(m_ctx.device, m_ctx.window);
        renderView.Reset();
        return;
    }

    // Render Frame
    PUSH_TRACE_SCOPE("Render");
    BeginGPULabel(cmd, "Frame");

    FrameContext frameCtx;
    frameCtx.cmd = cmd;
    frameCtx.deltaTime = deltaTime;
    frameCtx.frameIndex = m_ctx.currentFrame;
    frameCtx.swapchainTexture = swapchainTexture;
    frameCtx.swapchainWidth = swapchainTextureW;
    frameCtx.swapchainHeight = swapchainTextureH;
    frameCtx.previousPass = nullptr;
    frameCtx.resources = &m_compiledRes;

    for(std::unique_ptr<RenderPass>& renderPass : m_renderPasses) {
        renderPass->Render(frameCtx);
        frameCtx.previousPass = renderPass.get();
    }

    for(std::unique_ptr<RenderPass>& renderPass : m_renderPasses) {
        renderPass->Cleanup();
    }

    EndGPULabel(cmd);
    POP_TRACE_SCOPE();

    PUSH_TRACE_SCOPE("Submit");
    SDL_SubmitGPUCommandBuffer(cmd);

    SDL_WaitForGPUSwapchain(m_ctx.device, m_ctx.window);

    // Cleanup render view after frame is rendered
    renderView.Reset();

    m_ctx.currentFrame = (m_ctx.currentFrame + 1) % FRAMES_IN_FLIGHT;
    POP_TRACE_SCOPE();

    POP_TRACE_SCOPE();
}
