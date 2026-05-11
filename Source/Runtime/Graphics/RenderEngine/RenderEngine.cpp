#include "RenderEngine.h"

#include <Log/Log.h>

#include <Graphics/RenderPasses/OpaqueRenderPass.h>
#include <Graphics/RenderPasses/ReliefRenderPass.h>
#include <Graphics/RenderPasses/LightingRenderPass.h>
#include <Graphics/RenderPasses/UpscaleRenderPass.h>

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
    m_renderPasses[(uint32_t)RenderPassType::Opaque] = std::make_unique<OpaqueRenderPass>(m_ctx);
    m_renderPasses[(uint32_t)RenderPassType::Relief] = std::make_unique<ReliefRenderPass>(m_ctx);
    m_renderPasses[(uint32_t)RenderPassType::Lighting] = std::make_unique<LightingRenderPass>(m_ctx);
    m_renderPasses[(uint32_t)RenderPassType::Upscale] = std::make_unique<UpscaleRenderPass>(m_ctx);

    LOG(Log, LogRender, "Render Engine initialization complete");
}

void RenderEngine::Render(float deltaTime, RenderView &renderView)
{
    SDL_GPUCommandBuffer *cmd = SDL_AcquireGPUCommandBuffer(m_ctx.device);

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
    // ...
    EndGPULabel(cmd);

    // Execute OnDemand Tasks
    {
        // Lock OnDemand Task mutex so that no thread write to queue before we're finished
        std::unique_lock lock(m_onDemandMtx);
        BeginGPULabel(cmd, "On Demand Tasks");
        auto it = m_onDemandTasks.begin();
        while(it != m_onDemandTasks.end()) {
            (*it)->Execute();            
            it++;
        }
        m_onDemandTasks.clear();
        EndGPULabel(cmd);
    }

    // Sort & Compile elements
    {
        RenderResourceCompiler resourceCompiler(m_ctx, m_compiledRes);
        for(const auto& [id, rendObject] : renderView.m_dynamicRenderObjects) {
            for(const auto& rendElement : rendObject->GetElements()) {
                m_renderPasses[(uint32_t)rendElement->GetRenderPassType()]->Compile(resourceCompiler, rendObject.get(), rendElement.get());
            }
        }
    }

    // Render Frame
    BeginGPULabel(cmd, "Frame");

    FrameContext frameCtx;
    frameCtx.cmd = cmd;
    frameCtx.deltaTime = deltaTime;
    frameCtx.swapchainTexture = swapchainTexture;
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
}
