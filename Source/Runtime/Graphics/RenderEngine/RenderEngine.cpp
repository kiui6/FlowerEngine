#include "RenderEngine.h"

#include <Log/Log.h>

#include <Graphics/RenderPasses/OpaqueRenderPass/OpaqueRenderPass.h>
#include <Graphics/RenderPasses/ReliefRenderPass.h>
#include <Graphics/RenderPasses/LightingRenderPass.h>
#include <Graphics/RenderPasses/UpscaleRenderPass.h>
#include <Graphics/RenderPasses/DebugUIRenderPass/DebugUIRenderPass.h>

#include "RenderUtils.h"

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

    // Initialize Render Passes
    m_renderPasses[(uint32_t)RenderPassType::Opaque] = std::make_unique<OpaqueRenderPass>(m_ctx, m_stateStore);
    m_renderPasses[(uint32_t)RenderPassType::Relief] = std::make_unique<ReliefRenderPass>(m_ctx);
    m_renderPasses[(uint32_t)RenderPassType::Lighting] = std::make_unique<LightingRenderPass>(m_ctx);
    m_renderPasses[(uint32_t)RenderPassType::Upscale] = std::make_unique<UpscaleRenderPass>(m_ctx);
    m_renderPasses[(uint32_t)RenderPassType::DebugUI] = std::make_unique<DebugUIRenderPass>(m_ctx, m_stateStore);

    LOG(Log, LogRender, "Render Engine initialization complete");
}

void RenderEngine::Render(float deltaTime, RenderView &renderView)
{
    // Execute Render Jobs
    for(const auto& job : renderView.m_renderJobs) {
        job->Execute();
    }

    // Assemble elements
    RenderResourceCompiler resourceCompiler(m_ctx, m_compiledRes);
    for(const auto& [id, rendObject] : renderView.m_staticRenderObjects) {
        if(rendObject.lastReferencedFrames != 0) continue;

        for(const auto& rendElement : rendObject.object->GetElements()) {
            m_renderPasses[(uint32_t)rendElement->GetRenderPassType()]->Assemble(resourceCompiler, rendObject.object.get(), rendElement.get());
        }
    }

    for(const auto& [id, rendObject] : renderView.m_dynamicRenderObjects) {
        for(const auto& rendElement : rendObject->GetElements()) {
            m_renderPasses[(uint32_t)rendElement->GetRenderPassType()]->Assemble(resourceCompiler, rendObject.get(), rendElement.get());
        }
    }

    // Start recording main command buffer
    SDL_GPUCommandBuffer *cmd = SDL_AcquireGPUCommandBuffer(m_ctx.device);

    // After all data is assembled into batches, it can be compiled into GPU resources
    BeginGPULabel(cmd, "Compilation");
    SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(cmd);

    for(const auto& pass : m_renderPasses) {
        pass->Compile(cmd, copyPass);
    }

    SDL_EndGPUCopyPass(copyPass);
    EndGPULabel(cmd);

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

    // Perform State Changes
    BeginGPULabel(cmd, "State Changes");
    for(const auto& updateObj : renderView.m_stateUpdates) {
        updateObj->Apply(m_stateStore);
    }
    EndGPULabel(cmd);

    // Render Frame
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

    for(size_t passIdx = 0; passIdx < m_renderPasses.size(); passIdx++) {
        m_renderPasses[passIdx]->Render(frameCtx);
        frameCtx.previousPass = m_renderPasses[passIdx].get();
    }

    EndGPULabel(cmd);

    SDL_SubmitGPUCommandBuffer(cmd);

    SDL_WaitForGPUSwapchain(m_ctx.device, m_ctx.window);

    // Cleanup render view after frame is rendered
    renderView.Reset();

    m_ctx.currentFrame = (m_ctx.currentFrame + 1) % FRAMES_IN_FLIGHT;
}
