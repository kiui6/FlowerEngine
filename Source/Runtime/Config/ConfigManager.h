#pragma once

#include <Service/Service.h>

class ConfigManager : public IService
{
    static bool bIsInitialized;
public:
    static std::string_view GetStaticName() {return "ConfigManager";}

    virtual void Initialize() override;
    virtual void Deinitialize() override;
};