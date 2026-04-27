#include "AssetLibrary.h"

bool AssetLibrary::bIsInitialized = RegisterService<AssetLibrary>({DataManager::GetStaticName()});

void AssetLibrary::Initialize()
{
    std::string_view data = GetService<DataManager>()->GetName();
}

void AssetLibrary::Deinitialize()
{
}

AssetPtr<RawAsset> AssetLibrary::LoadRawAsset(const std::string& path)
{
    return nullptr;
}

void AssetLibrary::UnloadAsset(const std::string &path)
{
}

void AssetLibrary::DestroyAsset(Asset *asset)
{
}
