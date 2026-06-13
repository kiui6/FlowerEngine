#include "TaskPool.h"

#include <cassert>

TaskPool::TaskPool(size_t initialSize)
{
    for(int i = 0; i < initialSize; ++i) 
    {
        m_threads.emplace_back(&TaskPool::mf_taskThreadFunc, &m_tasks);
    }
}

void TaskPool::QueueTask(std::unique_ptr<IAsyncTask> &task, uint8_t priority)
{
    m_tasks.Push(task, priority);
}

void TaskPool::mf_taskThreadFunc(std::stop_token stopToken, ThreadDescriptor* self, TaskQueue* queue)
{
    while(!stopToken.stop_requested()) {
        std::unique_ptr<IAsyncTask> task = std::move(queue->Pop(stopToken));
        if(!task) continue;

        self->bInUse.store(true);

        task->Execute();

        self->bInUse.store(false);
    }
}

void TaskQueue::Push(std::unique_ptr<IAsyncTask> &task, uint8_t priority)
{
    std::lock_guard lock(m_queueLock);

    m_queue.emplace_front(TaskDescriptor(task, priority));

    m_cvTaskAvailable.notify_all();
}

std::unique_ptr<IAsyncTask> TaskQueue::Pop()
{
    {
        std::unique_lock taskAvailableLock(m_taskAvailableLock);
        m_cvTaskAvailable.wait(taskAvailableLock, [this] { return !m_queue.empty(); });
    }

    std::unique_lock lock(m_queueLock);

    std::unique_ptr<IAsyncTask> task = std::move(m_queue.front().task);

    // TODO: Priorities

    m_queue.pop_front();

    return std::move(task);
}

std::unique_ptr<IAsyncTask> TaskQueue::Pop(std::stop_token stopToken)
{
    {
        std::unique_lock taskAvailableLock(m_taskAvailableLock);
        if (!m_cvTaskAvailable.wait(taskAvailableLock, stopToken, [this] { return !m_queue.empty(); })) {
            return nullptr;
        }
    }

    std::unique_lock lock(m_queueLock);

    std::unique_ptr<IAsyncTask> task = std::move(m_queue.front().task);

    // TODO: Priorities

    m_queue.pop_front();

    return std::move(task);
}
