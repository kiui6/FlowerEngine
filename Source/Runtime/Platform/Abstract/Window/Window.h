#pragma once

#include <string>
#include <cstdint>
#include <vector>

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

#include <Delegate/Delegate.h>

#include "WindowEvents.h"

class Window
{    
    SDL_Window* m_window;
	SDL_Surface* m_surface;
    SDL_Event m_SDLEvent;

    std::vector<WindowEvent> m_winEvents{};

    std::string m_title;
    unsigned int m_width, m_height;
    bool m_sizeIsDirty = false;

    bool m_bShouldClose = false;
public:
    Window() = default;
    Window(std::string_view title, uint32_t width, uint32_t height) {Initialize(title, width, height);}
    void Initialize(std::string_view title, uint32_t width, uint32_t height);

    void CreateSurface(VkInstance instance, VkSurfaceKHR* vksurface);

    inline bool ShouldClose() const {return m_bShouldClose;}

    inline bool SizeChanged() const {return m_sizeIsDirty;}
    inline unsigned int GetWidth() const {return m_width;}
    inline unsigned int GetHeight() const {return m_height;}

    inline const std::vector<WindowEvent>& GetEvents() const {return m_winEvents;}

    void Update();
    void Cleanup();

    inline SDL_Window* GetSDLWindowHandle() const {return m_window;}

    /*
     * Window Event handler
     * Handles resize, minmax, repos scenarios
    */
    MulticastDelegate<Window*, const WindowEvent&> OnWindowEvent;

        /*
     * SDL Event Handler
     * Used for global SDL events, not tied to windows
    */
    MulticastDelegate<SDL_Event> OnSDLEvent;
};