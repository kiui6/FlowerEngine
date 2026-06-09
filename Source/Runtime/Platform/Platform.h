#pragma once

#include <Service/Service.h>

#include "PlatformDefines.h"

#include <string>
#include <memory>

#include "Abstract/Input/PlatformInput.h"
#include "Abstract/Filesystem/PlatformFilesystem.h"
#include "Abstract/System/PlatformSystem.h"

class Platform : public IService
{
    static bool bIsInitialized;

    std::unique_ptr<PlatformInput> m_input;
    std::unique_ptr<PlatformFilesystem> m_fs;
    std::unique_ptr<PlatformSystem> m_sys;
public:
    static std::string_view GetStaticName() {return "Platform";}

    virtual void Initialize() override;
    virtual void Deinitialize() override;

    static void DebugPrint(const char* string);

    PlatformInput* Input()              { return m_input.get();}
    PlatformFilesystem* Filesystem()    { return m_fs.get();}
    PlatformSystem* System()            { return m_sys.get();}
};