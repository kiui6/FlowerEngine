#pragma once

#include <Service/Service.h>

#include <Data/DataManager.h>

class AssetLibrary : public IService
{
    static bool bIsInitialized;
public:
    static std::string_view GetStaticName() {return "AssetLibrary";}

    virtual void Initialize() override;
    virtual void Deinitialize() override;
};