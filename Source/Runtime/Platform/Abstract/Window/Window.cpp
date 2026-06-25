#include "Window.h"

#include <Log/Log.h>

#include <Debug/Tracer/Tracer.h>

void Window::Initialize(std::string_view title, uint32_t width, uint32_t height)
{
	m_title = title;
    m_width = width;
    m_height = height;

	SDL_WindowFlags flags = SDL_WINDOW_RESIZABLE;

	#ifdef RENDER_USE_METAL
		flags |= SDL_WINDOW_METAL;
	#else
		flags |= SDL_WINDOW_VULKAN;
	#endif

    m_window = SDL_CreateWindow(m_title.c_str(), width, height, flags);

	m_surface = SDL_GetWindowSurface(m_window);

	SDL_UpdateWindowSurface(m_window);

	m_sizeIsDirty = true;
}

void Window::CreateSurface(VkInstance instance, VkSurfaceKHR *vksurface)
{
    if(SDL_Vulkan_CreateSurface(m_window, instance, nullptr, vksurface) == false)
	{
		LOG(Fatal, LogSDL, "Failed creating vulkan surface out of window.");
	}
	LOGF(Log, LogWindow, "Created surface: 0x%.16x", *vksurface);
}

void Window::Update()
{
	PUSH_TRACE_SCOPE("Window::Update()");

	while(SDL_PollEvent(&m_SDLEvent))
	{
		OnSDLEvent.Broadcast(m_SDLEvent);

		if(m_SDLEvent.window.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
			m_bShouldClose = true;
			return;
		}

		switch(m_SDLEvent.window.type)
		{
			case SDL_EVENT_WINDOW_RESIZED:
				m_width = m_SDLEvent.window.data1;
				m_height = m_SDLEvent.window.data2;
				m_sizeIsDirty = true;

				OnWindowEvent.Broadcast(this, 
					m_winEvents.emplace_back(WindowEvent{
						.type = WindowEventType::Resize,
						.parameters = { m_width, m_height }
					})
				);
				SDL_FlushEvent(SDL_EVENT_WINDOW_RESIZED);
				break;
			case SDL_EVENT_WINDOW_MINIMIZED:
				OnWindowEvent.Broadcast(this, 
					m_winEvents.emplace_back(WindowEvent{
						.type = WindowEventType::Minimize
					})
				);
				break;
			case SDL_EVENT_WINDOW_RESTORED:
				OnWindowEvent.Broadcast(this, 
					m_winEvents.emplace_back(WindowEvent{
						.type = WindowEventType::Exposed
					})
				);
				break;
			default:
				break;
		}
	}
	POP_TRACE_SCOPE();
}

void Window::Cleanup()
{
	m_sizeIsDirty = false;

	m_winEvents.clear();
}
