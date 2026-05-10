#include "RenderEngine.h"

#include <Log/Log.h>

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
    // TODO: m_renderPasses[(uint32_t)RenderPassType::Opaque] = std::make_unique<OpaqueRenderPass>();

    LOG(Log, LogRender, "Render Engine initialization complete");
}

void RenderEngine::Render(float deltaTime, RenderView &renderView)
{
    std::unique_lock lock(m_onDemandMtx);

    SDL_GPUCommandBuffer *cmd = SDL_AcquireGPUCommandBuffer(m_ctx.device);

    SDL_GPUTexture *swapchainTexture = NULL;
    uint32_t swapchainTextureW, swapchainTextureH;
    SDL_AcquireGPUSwapchainTexture(cmd, m_ctx.window, &swapchainTexture, &swapchainTextureW, &swapchainTextureH);

    // Perform State Changes

    // Execute OnDemand Tasks
    while(!m_onDemandTasks.empty()) {
        auto it = std::prev(m_onDemandTasks.end());
        (*it)->Execute();
        m_onDemandTasks.erase(it);
    }

    // Render Frame
    SDL_PushGPUDebugGroup(cmd, "Frame");
    // TODO: Remove this render pass
    SDL_GPUColorTargetInfo colorTarget = {
            .texture     = swapchainTexture,
            .mip_level   = 0,
            .layer_or_depth_plane = 0,
            .clear_color = (SDL_FColor){ 0.88f, 0.722f, 0.23f, 1.0f },
            .load_op     = SDL_GPU_LOADOP_CLEAR,
            .store_op    = SDL_GPU_STOREOP_STORE,
        };

    SDL_GPURenderPass* pass = SDL_BeginGPURenderPass(cmd, &colorTarget, 1, NULL);
    SDL_EndGPURenderPass(pass);

    for(size_t passIdx = 0; passIdx < m_renderPasses.size(); passIdx++) {
        // TODO: m_renderPasses[passIdx]->Render();
    }

    SDL_PopGPUDebugGroup(cmd);

    SDL_SubmitGPUCommandBuffer(cmd);

    SDL_WaitForGPUSwapchain(m_ctx.device, m_ctx.window);

    // Cleanup render view after frame is rendered
    renderView.Reset();
}
