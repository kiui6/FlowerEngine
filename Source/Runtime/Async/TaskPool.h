#pragma once

#include <thread>
#include <stop_token>
#include <vector>
#include <memory>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <any>
#include <atomic>
#include <future>
#include <queue>
#include <concepts>

#include "AsyncTask.h"

using AsyncTaskFunction = std::function<void(void)>;

template<typename T>
concept AsyncTaskClass = std::is_base_of<IAsyncTask, T>::value;

struct TaskDescriptor {
    uint8_t priority;
    std::unique_ptr<IAsyncTask> task;

    TaskDescriptor(std::unique_ptr<IAsyncTask>& newTask, uint8_t newPriority) : priority(newPriority), task(std::move(newTask)){}
};

class TaskQueue
{
    std::deque<TaskDescriptor> m_queue;
    std::mutex m_queueLock;
    std::mutex m_taskAvailableLock;
    std::condition_variable_any m_cvTaskAvailable;
public:
    void Push(std::unique_ptr<IAsyncTask>& task, uint8_t priority = 0);
    std::unique_ptr<IAsyncTask> Pop();
    std::unique_ptr<IAsyncTask> Pop(std::stop_token stopToken);

    size_t Size() const {return m_queue.size();}
};

class TaskPool
{
    struct ThreadDescriptor {
        std::jthread thread;
        std::atomic_bool bInUse = false;

        ThreadDescriptor(std::function<void(std::stop_token, ThreadDescriptor*, TaskQueue*)> threadCallback, TaskQueue* queue) : thread(threadCallback, this, queue) {}
        ThreadDescriptor(ThreadDescriptor&& other) noexcept
            : thread(std::move(other.thread)),
            bInUse(other.bInUse.load())
        {
            other.bInUse.store(false);
        }
    };

    std::mutex m_poolLock;

    TaskQueue m_tasks;
    std::vector<ThreadDescriptor> m_threads;
public:
    TaskPool(size_t initialSize);

    void QueueTask(std::unique_ptr<IAsyncTask> &task, uint8_t priority = 0);

private:
    static void mf_taskThreadFunc(std::stop_token stopToken, ThreadDescriptor* self, TaskQueue* queue);
};