#include <Application/Application.h>
#include <Platform/Platform.h>

// Dev


int GuardedMain(int argc, char* argv[])
{
    Application application;

    application.Initialize();

    LOG(Log, LogGuardedMain, "Starting application life cycle");

    {
        std::unique_ptr<World> myWorld = std::make_unique<World>();
        application.GetEngine()->TravelTo(std::move(myWorld));
    } 

    application.StartLifecycle();

    return 0;
}