#include "WritableSourceManager.h"

bool WritableSourceManager::bIsInitialized = RegisterService<WritableSourceManager>({DataManager::GetStaticName()});

void WritableSourceManager::Initialize()
{
}

void WritableSourceManager::Deinitialize()
{
}
