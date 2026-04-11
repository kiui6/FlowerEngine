#include "Window.h"

#include <Log/Log.h>

Window::Window(std::string_view title, uint32_t width, uint32_t height)
{
    m_title = title;
    m_width = width;
    m_height = height;

    m_window = SDL_CreateWindow(m_title.c_str(), width, height, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);

	m_surface = SDL_GetWindowSurface(m_window);

	SDL_UpdateWindowSurface(m_window);
}

void Window::CreateSurface(VkInstance instance, VkSurfaceKHR* vksurface)
{
    if(SDL_Vulkan_CreateSurface(m_window, instance, nullptr, vksurface) == false)
	{
		LOG(Fatal, LogSDL, "Failed creating vulkan surface out of window.");
	}
	LOGF(Log, LogWindow, "Created surface: 0x%.16x", *vksurface);
}

void Window::Update()
{
	while(SDL_PollEvent(&m_event))
	{
		OnSDLEvent.Broadcast(m_event);

		switch(m_event.window.type)
		{
        	case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
        	    m_bShouldClose = true;
				break;
			case SDL_EVENT_WINDOW_RESIZED:
        	    if(!m_bShouldClose) {
					m_width = m_event.window.data1;
					m_height = m_event.window.data2;
				    OnWindowEvent.Broadcast(this, new WindowEventPayloadResize(m_event.window.data1, m_event.window.data2));
					SDL_FlushEvent(SDL_EVENT_WINDOW_RESIZED);
        	    }
				break;
			case SDL_EVENT_WINDOW_MINIMIZED:
				if(!m_bShouldClose) {
				    OnWindowEvent.Broadcast(this, new WindowEventPayload(WINDOW_EVENT_HIDDEN));
        	    }
				break;
			case SDL_EVENT_WINDOW_RESTORED:
				if(!m_bShouldClose) {
				    OnWindowEvent.Broadcast(this, new WindowEventPayloadResize(m_width, m_height));
        	    }
				break;
			default:
				break;
		}
	}
	
}