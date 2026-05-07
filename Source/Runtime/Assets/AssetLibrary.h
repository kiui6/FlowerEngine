#pragma once

#include <Service/Service.h>
#include <Data/DataManager.h>

#include "RawAsset.h"
#include "AssetPtr.h"

#include <memory>
#include <string>

class AssetLibrary : public IService
{
    static bool bIsInitialized;

    std::unordered_map<std::string, std::unique_ptr<Asset>> m_loadedAssets;
    std::vector<std::unique_ptr<Asset>> m_createdAssets;

public:
    static std::string_view GetStaticName() {return "AssetLibrary";}

    virtual void Initialize() override;
    virtual void Deinitialize() override;

    AssetPtr<RawAsset> LoadRawAsset(std::string_view path);

    template <AssetClass T>
    AssetPtr<T> LoadAsset(const std::string& path);

    void UnloadAsset(const std::string& path);

    template <AssetClass T>
    T CreateAsset();

    void DestroyAsset(Asset* asset);
};