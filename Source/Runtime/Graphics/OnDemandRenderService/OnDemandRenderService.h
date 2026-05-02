#pragma once

#include <Service/Service.h>
#include <Graphics/RenderEngine/OnDemand/OnDemandRenderTask.h>
#include <Graphics/RenderEngine/RenderEngine.h>

#include <Application/Application.h>
#include <Log/Log.h>

class OnDemandRenderService : public IService
{
    static bool bIsInitialized;
public:
    static std::string_view GetStaticName() {return "OnDemandRenderService";}

    virtual void Initialize() override {}
    virtual void Deinitialize() override {}

    template <OnDemandRenderTaskClass T, typename... Args>
    void Submit(Args&&... args) {
        RenderEngine* re = Application::Get()->GetRenderEngine();
        if(!re) {
            LOG(Error, LogOnDemandRenderService, "Submit() failed to retrieve RenderEngine.");
            return;
        }

        re->SubmitOnDemandTask(new T(std::forward<Args>(args)...));
    }
};