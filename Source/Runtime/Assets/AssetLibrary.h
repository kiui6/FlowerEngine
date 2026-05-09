#pragma once

#include <Service/Service.h>
#include <Data/DataManager.h>

#include "RawAsset.h"
#include "AssetPtr.h"
#include "AssetFactory.h"

#include <memory>
#include <string>

#include <Log/Log.h>

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
    AssetPtr<T> LoadAsset(std::string_view path);

    void UnloadAsset(const std::string& path);

    template <AssetClass T>
    T* CreateAsset();

    void DestroyAsset(Asset* asset);
};

template <AssetClass T>
inline AssetPtr<T> AssetLibrary::LoadAsset(std::string_view path)
{
    DataView view = GetService<DataManager>()->OpenDataView(path);
    
    std::unique_ptr<T> asset = std::make_unique<T>(path, view);

    auto pair = m_loadedAssets.emplace(path, std::move(asset));

    LOGF(Log, LogAssetLibrary, "Loaded Asset[%s] of Type[%c%c%c%c]", pair.first->second->GetPath().c_str(), T::StaticType(), T::StaticType() >> 8, T::StaticType() >> 16, T::StaticType() >> 24);

    return AssetPtr<T>(path, static_cast<T*>(pair.first->second.get()));
}
