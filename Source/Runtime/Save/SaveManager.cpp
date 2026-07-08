#include "SaveManager.h"

bool SaveManager::bIsInitialized = RegisterRuntimeService<SaveManager>({WritableSourceManager::GetStaticName()});

void SaveManager::Initialize()
{
}

void SaveManager::Deinitialize()
{
}
