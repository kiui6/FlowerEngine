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

    template <AsyncTaskClass Task, typename... Args>
    void EnqueueTask(uint8_t priority, Args&&... args);
};

template <AsyncTaskClass Task, typename... Args>
inline void AsyncService::EnqueueTask(uint8_t priority, Args&&... args)
{
    std::unique_ptr<IAsyncTask> task = std::make_unique<Task>(std::forward<Args>(args)...);
    m_taskPool.QueueTask(task, priority);
}
