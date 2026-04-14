#include "AssetLibrary.h"

bool AssetLibrary::bIsInitialized = RegisterService<AssetLibrary>({DataManager::GetStaticName()});

void AssetLibrary::Initialize()
{
    std::string_view data = GetService<DataManager>()->GetName();
}

void AssetLibrary::Deinitialize()
{
}
