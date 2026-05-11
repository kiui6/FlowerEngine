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

void AssetLibrary::UnloadAsset(const std::string &path)
{
}

void AssetLibrary::DestroyAsset(Asset *asset)
{
}
