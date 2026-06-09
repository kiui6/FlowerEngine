#include "Platform.h"

#include <cassert>
#include <cstdlib>

#include <Log/Log.h>

#include <SDL3/SDL.h>

bool Platform::bIsInitialized = RegisterService<Platform>();

#ifdef PLATFORM_WINDOWS
#   include "Windows/Input/WindowsPlatformInput.h"
#   include "Windows/Filesystem/WindowsPlatformFilesystem.h"
#   include "Windows/System/WindowsPlatformSystem.h"
#elifdef PLATFORM_LINUX

#elifdef PLATFORM_UNIX

#elifdef PLATFORM_MAC

#endif

void Platform::Initialize()
{
    // Initialize SDL framework
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_GAMEPAD) == false)
	{
		assert(!"SDL Initialization failed");
        std::abort();
	}

    // Initialize subsystems
#ifdef PLATFORM_WINDOWS 
    m_input = std::make_unique<WindowsPlatformInput>();
    m_fs = std::make_unique<WindowsPlatformFilesystem>();
    m_sys = std::make_unique<WindowsPlatformSystem>();
#endif


    LOGF(Log, LogPlatform, "Initialized platform: %s", m_sys->PlatformName().data());
}

void Platform::Deinitialize()
{
    
}