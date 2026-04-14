#include <Application/Application.h>
#include <Platform/Platform.h>

// Dev
#include <future>
#include <Async/AsyncService.h>

class TestAsync : public IAsyncTask
{
    std::promise<int> promise;
public:
    TestAsync(std::promise<int> _promise) : promise(std::move(_promise)) {}

    virtual void Execute() override 
    {
        LOG(Log, LogAsync, "Executed async task!");
        promise.set_value(12);
    }
};

int GuardedMain(int argc, char* argv[])
{
    Application application;

    application.Initialize();

    LOG(Log, LogGuardedMain, "Starting application life cycle");

    std::promise<int> promise;
    std::future<int> future = promise.get_future();
    GetService<AsyncService>()->EnqueueTask<TestAsync>(0, std::move(promise));

    application.StartLifecycle();

    int futureVal = future.get();

    return 0;
}