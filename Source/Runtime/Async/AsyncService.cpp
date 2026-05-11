#include "AsyncService.h"

#include <Config/Config.h>

bool AsyncService::bIsInitialized = RegisterService<AsyncService>({Config::GetStaticName()});

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