#include "LocaleManager.h"

#include <Assets/AssetLibrary.h>
#include <Assets/RawAsset.h>

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

    AssetPtr<RawAsset> localeFile = lib->LoadAsset<RawAsset>(localeName);
}

std::optional<std::string_view> LocaleManager::FetchLocaleString(std::string_view id)
{
    return std::optional<std::string_view>();
}
