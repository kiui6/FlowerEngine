#include "Application.h"

#include <ctime>
#include <chrono>
#include <cassert>

#include <Service/Service.h>
#include <Config/Config.h>

#include <Platform/Platform.h>

#include <Graphics/RenderStateUpdates/DebugUIStateUpdate.h>
#include <Graphics/RenderStateUpdates/GlobalStateUpdate.h>
#include <Graphics/RenderStateUpdates/UpscaleStateUpdate.h>

//#include <Asset/AssetLoader.h>
//#include <Record/ObjectLibrary.h>
//#include <Thread/ThreadManager.h>

#include <Debug/Tracer/Tracer.h>

Application* Application::Singleton = nullptr;

Application::Application()
{
    if(Application::Singleton != nullptr) {
        assert(!"Only one application class should exist at the same time");
		return;
	}

    Application::Singleton = this;
}

Application::~Application()
{
    ServiceProvider::Get().Deinitialize();

    if(Application::Singleton == this)
    {
        Application::Singleton = nullptr;
    }
}

void Application::SetDebugWindow(DebugWindow *window)
{
    m_dbgWindow = std::shared_ptr<DebugWindow>(window);
    m_dbgWindowUpdated = true;
}

void Application::Initialize()
{
    ServiceProvider::Get().Initialize();

    m_inputDev = GetService<Platform>()->Input()->CreateRawInputDevice();

    // Create window
    m_window.Initialize("Flower++", 1280, 720);
	
	m_render.Initialize(m_window.GetSDLWindowHandle());

	LOG(Log, LogApplication, "Successfully initialized Application's Essentials");
}

void Application::StartLifecycle()
{
    // Allow running
    m_bRunning.store(true);

    // Initialize Engine's essentials and start game mode
    m_engine.Initialize();
    
    auto frame_lifetime_start = std::chrono::high_resolution_clock::now();
	auto frame_lifetime_end = std::chrono::high_resolution_clock::now();

#	ifdef EDITOR
	float MinimalDeltaTime = 1000.0f/120.0f;
#	else
    float MinimalDeltaTime = 1000.0f/120.0f;
	// float MinimalDeltaTime = 1000.0f/static_cast<float>(GetService<SettingsManager>()->GetOrDefault<GameSettings>()->MaxFPS);
#	endif

	while(!m_window.ShouldClose() && m_bRunning)
	{
        PUSH_TRACE_SCOPE("Application::StartLifecycle()");
        m_deltaTime = std::chrono::duration<float, std::milli>(frame_lifetime_end-frame_lifetime_start).count() / 1000;

		frame_lifetime_start = std::chrono::high_resolution_clock::now();

        PUSH_TRACE_SCOPE("Minimal DeltaTime");
		if (m_deltaTime < MinimalDeltaTime)
        {
            std::chrono::duration<double, std::milli> delta_ms(MinimalDeltaTime - m_deltaTime);
            auto delta_ms_duration = std::chrono::duration_cast<std::chrono::milliseconds>(delta_ms);
            std::this_thread::sleep_for(std::chrono::milliseconds(delta_ms_duration.count()));
        }
        POP_TRACE_SCOPE();

		m_window.Update();

        m_engine.Tick(m_deltaTime);
        
        RenderView& renderView = m_render.GetFrameRenderView();
        // Record state changes
        RecordRenderStateUpdates(renderView);
        // Record from engine
        m_engine.RecordRenderView(renderView);
        m_render.Render(m_deltaTime, renderView);

        // Clear window event flags after we record render state updates
        // Otherwise resized flag will be cleared on first frame, preventing upscale state initialization
        m_window.Cleanup();

        frame_lifetime_end = std::chrono::high_resolution_clock::now();
        POP_TRACE_SCOPE();
        FLUSH_TRACE_FRAME();
	}

    LOG(Log, LogApplication, "Game Thread is stopping");

    // Application can't run anymore and should quit
    m_bRunning.store(false);
}

void Application::Stop()
{
}

void Application::RecordRenderStateUpdates(RenderView & view)
{
    if(m_dbgWindowUpdated) {
        view.GetStateUpdate<DebugUIStateUpdate>()->window = m_dbgWindow;
        m_dbgWindowUpdated = false;
    }

    if(m_window.SizeChanged()) {
        view.GetStateUpdate<UpscaleStateUpdate>()->SetViewport(m_window.GetWidth(), m_window.GetHeight());
    }
}
