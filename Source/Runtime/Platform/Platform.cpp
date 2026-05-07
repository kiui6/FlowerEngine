#include "Platform.h"

#include <cassert>

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

std::shared_ptr<File> Platform::OpenFile(std::string path, FileAccess access)
{
    std::shared_ptr<File> file = std::make_shared<File>();
    file->Open(path, access);
    return file;
}

std::shared_ptr<IMappedFile> Platform::MapFile(std::string path, FileAccess access)
{
    assert(m_impl != nullptr);

    std::shared_ptr<IMappedFile> file = std::move(m_impl->CreateMappedFile());
    file->Open(path, access);
    return file;
}
