#pragma once

#include <Service/Service.h>

#include "PlatformDefines.h"
#include "PlatformImpl.h"

#include "File.h"
#include "MappedFile.h"

#include <string>
#include <memory>

class Platform : public IService
{
    static bool bIsInitialized;

    std::string m_basePath;
    std::string m_prefPath;
    
    std::unique_ptr<IPlatformImpl> m_impl;
public:
    static void DebugPrint(const char* string);

    static std::string_view GetStaticName() {return "Platform";}

    virtual void Initialize() override;
    virtual void Deinitialize() override;

    std::string GetLocalDirectory();

    File OpenFile(std::string path, uint8_t ioaccess);
    std::unique_ptr<IMappedFile> MapFile(std::string path, uint8_t ioaccess);
};