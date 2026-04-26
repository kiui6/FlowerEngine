#include "Application.h"

#include <ctime>
#include <chrono>
#include <cassert>

#include <Service/Service.h>
#include <Config/ConfigManager.h>

#include <Platform/Platform.h>

//#include <Asset/AssetLoader.h>
//#include <Record/ObjectLibrary.h>
//#include <Thread/ThreadManager.h>

#include <Graphics/Render/Utility.h>

#include <SDL3/SDL.h>

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
    if(Application::Singleton == this)
    {
        Application::Singleton = nullptr;
    }

    ServiceProvider::Get().Deinitialize();
}

void Application::Initialize()
{
    ServiceProvider::Get().Initialize();

    // Create Engine
    m_engine = std::make_unique<Engine>();
    
    // Create Render Engine object
    m_render = std::make_unique<RenderEngine>();

    // Initialize SDL framework
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_GAMEPAD) == false)
	{
		LOG(Fatal, SDL, "SDL Initialization failed");
	}

    // Create window
    m_window = std::make_unique<Window>("Flower++", 1280, 720);
	
	// Render->Initialize(SDLExtensions, InstanceLayers);

	// SDL_Vulkan_CreateSurface(m_window->GetSDLWindowHandle(), Render->GetInstance(), nullptr, &dummySurface);

	// Render->UpdateQueueFamilies(dummySurface);

    // Render->CreateDevice();

    // IRenderUtility::CreateSingleTimeCommandPool();

	LOG(Log, LogApplication, "Successfully initialized Application's Essentials");
}

void Application::StartLifecycle()
{
    // Allow running
    m_bRunning.store(true);
    
    auto frame_lifetime_start = std::chrono::high_resolution_clock::now();
	auto frame_lifetime_end = std::chrono::high_resolution_clock::now();

#	ifdef EDITOR
	float MinimalDeltaTime = 1000.0f/120.0f;
#	else
    float MinimalDeltaTime = 1000.0f/120.0f;
	// float MinimalDeltaTime = 1000.0f/static_cast<float>(GetService<SettingsManager>()->GetOrDefault<GameSettings>()->MaxFPS);
#	endif

	while(!m_window->ShouldClose() && m_bRunning)
	{
        m_deltaTime = std::chrono::duration<float, std::milli>(frame_lifetime_end-frame_lifetime_start).count() / 1000;

		frame_lifetime_start = std::chrono::high_resolution_clock::now();

		if (m_deltaTime < MinimalDeltaTime)
        {
            std::chrono::duration<double, std::milli> delta_ms(MinimalDeltaTime - m_deltaTime);
            auto delta_ms_duration = std::chrono::duration_cast<std::chrono::milliseconds>(delta_ms);
            std::this_thread::sleep_for(std::chrono::milliseconds(delta_ms_duration.count()));
        }

		m_window->Update();

        m_engine->Tick(m_deltaTime);
        
        RenderView& renderView = m_render->GetFrameRenderView();
        m_engine->RecordRenderView(renderView);
        m_render->Render(m_deltaTime, renderView);

        frame_lifetime_end = std::chrono::high_resolution_clock::now();
	}

    LOG(Log, LogApplication, "Game Thread is stopping");

    // Application can't run anymore and should quit
    m_bRunning.store(false);
}

void Application::Stop()
{
}