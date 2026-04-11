#include <Application/Application.h>
#include <Platform/Platform.h>

#include <Async/AsyncService.h>

class TestAsync : public IAsyncTask
{
public:
    virtual void Execute() override 
    {
        LOG(Log, LogAsync, "Executed async task!");
    }
};

int GuardedMain(int argc, char* argv[])
{
    Application application;

    application.Initialize();

    LOG(Log, LogGuardedMain, "Starting application life cycle");

    GetService<AsyncService>()->EnqueueTask<TestAsync>();

    application.StartLifecycle();

    return 0;
}