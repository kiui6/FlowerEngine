#include "LocaleManager.h"

#include <Data/DataManager.h>

bool LocaleManager::bIsInitialized = RegisterService<LocaleManager>({DataManager::GetStaticName()});

void LocaleManager::Initialize()
{
}

void LocaleManager::Deinitialize()
{
}

std::optional<std::string_view> LocaleManager::FetchLocaleString(std::string_view id)
{
    return std::optional<std::string_view>();
}
