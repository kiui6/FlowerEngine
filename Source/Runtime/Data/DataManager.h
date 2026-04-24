#pragma once

#include <Service/Service.h>

#include <optional>

#include "Formats/MasterFile/MasterFile.h"
#include "Formats/PluginFile/PluginFile.h"

class DataManager : public IService
{
    static bool bIsInitialized;
public:
    static std::string_view GetStaticName() {return "DataManager";}

    virtual void Initialize() override;
    virtual void Deinitialize() override;

    
};