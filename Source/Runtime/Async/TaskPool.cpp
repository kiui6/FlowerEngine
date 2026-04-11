#include "TaskPool.h"

#include <cassert>

TaskPool::TaskPool(size_t initialSize)
{
    for(int i = 0; i < initialSize; ++i) 
    {
        m_threads.emplace_back(std::make_unique<ThreadDescriptor>(&TaskPool::mf_taskThreadFunc, &m_tasks));
    }
}

void TaskPool::QueueTask(std::unique_ptr<IAsyncTask> &task, uint8_t priority)
{
    std::lock_guard mtx(m_poolLock);

    m_tasks.Push(task, priority);



    // 
}

void TaskPool::mf_taskThreadFunc(std::stop_token stopToken, ThreadDescriptor* self, TaskQueue* queue)
{
    while(!stopToken.stop_requested()) {
        std::unique_ptr<IAsyncTask> task = std::move(queue->Pop());
        if(task == nullptr) {
            std::unique_lock<std::mutex> lock(self->mtxTaskAvailable);
            queue->TaskAvailableCV().wait(lock, [queue](){return queue->Size();});
            continue;
        }

        self->bInUse.store(true);

        task->Execute();

        self->bInUse.store(false);
    }
}

void TaskQueue::Push(std::unique_ptr<IAsyncTask> &task, uint8_t priority)
{
    std::lock_guard lock(m_queueLock);

    m_queue.emplace_front(TaskDescriptor(task, priority));

    cvTaskAvailable.notify_all();
}

std::unique_ptr<IAsyncTask> TaskQueue::Pop()
{
    std::lock_guard lock(m_queueLock);

    if(m_queue.size() <= 0) {
        return nullptr;
    }

    std::unique_ptr<IAsyncTask> task = std::move(m_queue.front().task);

    // TODO: Priorities

    m_queue.pop_front();

    return std::move(task);
}
