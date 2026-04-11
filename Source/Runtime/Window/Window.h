#pragma once

#include <string>
#include <cstdint>

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

#include <Delegate/Delegate.h>

#include "WindowEvents.h"

class Window
{    
    SDL_Window* m_window;
	SDL_Surface* m_surface;
    SDL_Event m_event;

    std::string m_title;
    unsigned int m_width, m_height;

    bool m_bShouldClose = false;
protected:
    uint32_t m_rendererCompositionID;

    friend class Application;
public:
    Window(std::string_view title, uint32_t width, uint32_t height);

    void CreateSurface(VkInstance instance, VkSurfaceKHR* vksurface);

    inline bool ShouldClose() const {return m_bShouldClose;}

    void Update();

    inline SDL_Window* GetSDLWindowHandle() const {return m_window;}

    uint32_t GetCompositionID() const {return m_rendererCompositionID;}

    /*
     * Window Event handler
     * Handles resize, minmax, repos scenarios
    */
    MulticastDelegate<Window*, WindowEventPayload*> OnWindowEvent;

        /*
     * SDL Event Handler
     * Used for global SDL events, not tied to windows
    */
    MulticastDelegate<SDL_Event> OnSDLEvent;
};