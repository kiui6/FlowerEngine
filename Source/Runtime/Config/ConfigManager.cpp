#include "ConfigManager.h"

#include <Platform/Platform.h>

bool ConfigManager::bIsInitialized = RegisterService<ConfigManager>({Platform::GetStaticName()});

void ConfigManager::Initialize()
{
}

void ConfigManager::Deinitialize()
{
}
