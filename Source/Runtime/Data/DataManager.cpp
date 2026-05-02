#include "DataManager.h"

#include <Platform/Platform.h>
#include <Async/AsyncService.h>

#include "Formats/MasterFile/MasterFile.h"
#include "Formats/PluginFile/PluginFile.h"

bool DataManager::bIsInitialized = RegisterService<DataManager>({Platform::GetStaticName(), AsyncService::GetStaticName()});

void DataManager::Initialize()
{
}

void DataManager::Deinitialize()
{
}

File DataManager::OpenAssetFile(std::string relativePath)
{
    return GetService<Platform>()->OpenFile(PathBuilder::MakeAbsolute(m_basePath, relativePath), 0);
}
