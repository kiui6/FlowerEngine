#pragma once

#include <Service/Service.h>
#include <Data/DataManager.h>

#include "RawAsset.h"

#include <memory>
#include <string>

class AssetLibrary : public IService
{
    static bool bIsInitialized;

    std::unordered_map<std::string, std::shared_ptr<Asset>> m_loadedAssets;

public:
    static std::string_view GetStaticName() {return "AssetLibrary";}

    virtual void Initialize() override;
    virtual void Deinitialize() override;

    std::shared_ptr<RawAsset> LoadRawAsset(const std::string& path);
};