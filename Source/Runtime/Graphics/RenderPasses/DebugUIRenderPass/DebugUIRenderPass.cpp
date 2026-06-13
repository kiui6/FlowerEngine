#include "DebugUIRenderPass.h"

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_sdl3.h>
#include <imgui/backends/imgui_impl_sdlgpu3.h>

#include <Application/Application.h>
#include <Debug/Tracer/Tracer.h>

DebugUIRenderPass::DebugUIRenderPass(GPUContext &context, RenderStateStore& stateStore)
    : m_gpu(context), m_state(stateStore.Get<DebugUIRenderState>())
{
    m_state.SubscribeToChange(this, &DebugUIRenderPass::cbOnChange);

    #ifdef HAS_DEBUGUI
    
    // FIXME: Maybe we can come up with more graceful way of intercepting SDL events
    Application::Get()->GetWindow().OnSDLEvent.BindRaw([](const SDL_Event& event){ImGui_ImplSDL3_ProcessEvent(&event);});

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = "./Config/imgui.ini";
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui_ImplSDL3_InitForSDLGPU(context.window);
    ImGui_ImplSDLGPU3_InitInfo init_info = {};
    init_info.Device = context.device;
    init_info.ColorTargetFormat = SDL_GetGPUSwapchainTextureFormat(context.device, context.window);
    init_info.MSAASamples = SDL_GPU_SAMPLECOUNT_1;
    init_info.SwapchainComposition = SDL_GPU_SWAPCHAINCOMPOSITION_SDR;
    init_info.PresentMode = SDL_GPU_PRESENTMODE_VSYNC;
    ImGui_ImplSDLGPU3_Init(&init_info);
    #endif
}

DebugUIRenderPass::~DebugUIRenderPass()
{
}

void DebugUIRenderPass::RenderFrame(FrameContext &ctx)
{
    PUSH_TRACE_SCOPE("DebugUIRenderPass::Render()");
#ifdef HAS_DEBUGUI
    ImGui_ImplSDLGPU3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    if(std::shared_ptr<DebugWindow> winPtr = m_state.window.lock()) {
        winPtr->Render();
    }

    ImGui::Render();
    ImDrawData* drawData = ImGui::GetDrawData();

    if (ctx.swapchainTexture != nullptr)
    {
        ImGui_ImplSDLGPU3_PrepareDrawData(drawData, ctx.cmd);

        SDL_GPUColorTargetInfo target_info = {};
        target_info.texture = ctx.swapchainTexture;
        target_info.load_op = SDL_GPU_LOADOP_LOAD;
        target_info.store_op = SDL_GPU_STOREOP_STORE;
        target_info.mip_level = 0;
        target_info.layer_or_depth_plane = 0;
        target_info.cycle = false;
        SDL_GPURenderPass* render_pass = SDL_BeginGPURenderPass(ctx.cmd, &target_info, 1, nullptr);

        ImGui_ImplSDLGPU3_RenderDrawData(drawData, ctx.cmd, render_pass);

        SDL_EndGPURenderPass(render_pass);

        //ImGui::UpdatePlatformWindows();
        //ImGui::RenderPlatformWindowsDefault();
    }
#endif
    POP_TRACE_SCOPE();
}

void DebugUIRenderPass::cbOnChange()
{
}
