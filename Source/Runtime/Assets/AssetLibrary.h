#pragma once

#include <Service/Service.h>
#include <Data/DataManager.h>

#include "AssetPtr.h"
#include "AssetFactory.h"

#include <memory>
#include <string>

#include <Log/Log.h>
#include <Utility/Hash.h>

#include <Platform/PlatformDefines.h>

class AssetLibrary : public IService
{
    static bool bIsInitialized;

    std::unordered_map<std::string, std::unique_ptr<Asset>, StringHash, std::equal_to<>> m_loadedAssets;
    std::vector<std::unique_ptr<Asset>> m_createdAssets;

public:
    static std::string_view GetStaticName() {return "AssetLibrary";}

    virtual void Initialize() override;
    virtual void Deinitialize() override;

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
    auto loadedAsset = m_loadedAssets.find(path);
    if(loadedAsset != m_loadedAssets.end()) {
        if(loadedAsset->second->GetType() != T::StaticType()) {
            ID32 loadedType = loadedAsset->second->GetType();
            LOGF(Error, LogAssetLibrary, "Failed to load Asset[%s] of Type[%c%c%c%c]: Already loaded with Type[%c%c%c%c]", path.data(), 
                T::StaticType(), T::StaticType() >> 8, T::StaticType() >> 16, T::StaticType() >> 24,
                loadedType, loadedType >> 8, loadedType >> 16, loadedType >> 24);
            return AssetPtr<T>(path, nullptr);
        }

        return AssetPtr<T>(path, static_cast<T*>(loadedAsset->second.get()));
    }

    // TODO: Refactor this, too much ambiguity
    FileView view = GetService<DataManager>()->OpenFileRead(path);
    
    std::unique_ptr<T> asset = std::make_unique<T>(path, view.MakeView());

    if(!asset || !asset->IsValid()) {
        LOGF(Error, LogAssetLibrary, "Failed to load Asset[%s] of Type[%c%c%c%c]", path.data(), T::StaticType(), T::StaticType() >> 8, T::StaticType() >> 16, T::StaticType() >> 24);
        return AssetPtr<T>(path, nullptr);
    }

    auto pair = m_loadedAssets.emplace(path, std::move(asset));

    if constexpr(IS_VERBOSE) {
        LOGF(Log, LogAssetLibrary, "Loaded Asset[%s] of Type[%c%c%c%c]", pair.first->second->GetPath().c_str(), T::StaticType(), T::StaticType() >> 8, T::StaticType() >> 16, T::StaticType() >> 24);
    }

    return AssetPtr<T>(path, static_cast<T*>(pair.first->second.get()));
}
