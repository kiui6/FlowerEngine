#include "WritableSourceManager.h"

#include <Record/RecordLibrary.h>

bool WritableSourceManager::bIsInitialized = RegisterService<WritableSourceManager>({DataManager::GetStaticName(), RecordLibrary::GetStaticName()});

void WritableSourceManager::Initialize()
{
}

void WritableSourceManager::Deinitialize()
{
}
