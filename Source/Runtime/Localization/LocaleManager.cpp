#include "LocaleManager.h"

#include <Assets/AssetLibrary.h>

bool LocaleManager::bIsInitialized = RegisterService<LocaleManager>({AssetLibrary::GetStaticName()});

void LocaleManager::Initialize()
{
}

void LocaleManager::Deinitialize()
{
}

void LocaleManager::LoadLocale(std::string localeName)
{
    AssetLibrary* lib = GetService<AssetLibrary>();

    std::shared_ptr<RawAsset> localeFile = lib->LoadRawAsset(localeName);
}

std::optional<std::string_view> LocaleManager::FetchLocaleString(std::string_view id)
{
    return std::optional<std::string_view>();
}
