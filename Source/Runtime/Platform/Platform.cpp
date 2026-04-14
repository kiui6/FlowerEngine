#include "Platform.h"

#include <Log/Log.h>

bool Platform::bIsInitialized = RegisterService<Platform>();

#ifdef PLATFORM_WINDOWS
#   include "Windows/WindowsPlatform.h"
#elifdef PLATFORM_LINUX

#elifdef PLATFORM_UNIX

#elifdef PLATFORM_MAC

#endif

void Platform::Initialize()
{
    m_impl = std::make_unique<PlatformImpl>();

    LOGF(Log, LogPlatform, "Initialized platform: %s", m_impl->GetPlatformName().data());
}

void Platform::Deinitialize()
{
    
}
