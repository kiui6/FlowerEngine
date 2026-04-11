#include "AsyncService.h"

bool AsyncService::bIsInitialized = RegisterService<AsyncService>();

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