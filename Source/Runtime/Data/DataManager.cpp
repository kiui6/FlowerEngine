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