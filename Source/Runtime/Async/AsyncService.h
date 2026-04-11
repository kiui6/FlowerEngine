#pragma once 

#include <Service/Service.h>

#include "TaskPool.h"

class AsyncService : public IService
{
    static bool bIsInitialized;

    TaskPool m_taskPool;
public:
    AsyncService();

    static std::string_view GetStaticName() {return "AsyncService";}

    virtual void Initialize() override;
    virtual void Deinitialize() override;

    template <AsyncTaskClass _TTask>
    void EnqueueTask(uint8_t priority = 0);
};

template <AsyncTaskClass _TTask>
inline void AsyncService::EnqueueTask(uint8_t priority)
{
    std::unique_ptr<IAsyncTask> task = std::make_unique<_TTask>();
    m_taskPool.QueueTask(task, priority);
}
