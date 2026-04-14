#include "AsyncService.h"

#include <Config/ConfigManager.h>

bool AsyncService::bIsInitialized = RegisterService<AsyncService>({ConfigManager::GetStaticName()});

AsyncService::AsyncService()
    : m_taskPool(4)
{
}

void AsyncService::Initialize()
{
}

void AsyncService::Deinitialize()
{
}