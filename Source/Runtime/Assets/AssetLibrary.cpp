#include "AssetLibrary.h"

#include "AssetFactory.h"

#include <Log/Log.h>

bool AssetLibrary::bIsInitialized = RegisterService<AssetLibrary>({DataManager::GetStaticName()});

void AssetLibrary::Initialize()
{
    std::string_view data = GetService<DataManager>()->GetName();
}

void AssetLibrary::Deinitialize()
{
}

AssetPtr<RawAsset> AssetLibrary::LoadRawAsset(std::string_view path)
{
    DataView view = GetService<DataManager>()->OpenDataView(path);
    
    std::unique_ptr<RawAsset> asset = std::make_unique<RawAsset>(path, view);

    auto pair = m_loadedAssets.emplace(path, std::move(asset));

    LOGF(Log, LogAssetLibrary, "Loaded asset: %s", pair.first->second->GetPath().c_str());

    return AssetPtr<RawAsset>(static_cast<RawAsset*>(pair.first->second.get()));
}

void AssetLibrary::UnloadAsset(const std::string &path)
{
}

void AssetLibrary::DestroyAsset(Asset *asset)
{
}
