#include "AssetLibrary.h"

bool AssetLibrary::bIsInitialized = RegisterService<AssetLibrary>({DataManager::GetStaticName()});

void AssetLibrary::Initialize()
{
    std::string_view data = GetService<DataManager>()->GetName();
}

void AssetLibrary::Deinitialize()
{
}

std::shared_ptr<RawAsset> AssetLibrary::LoadRawAsset(const std::string &path)
{
    return std::shared_ptr<RawAsset>();
}
