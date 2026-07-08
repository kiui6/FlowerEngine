#pragma once

#include <Service/Service.h>

#include <optional>
#include <memory>
#include <atomic>
#include <shared_mutex>
#include <mutex>

#include <Data/DataManager.h>

class WritableSourceManager : public IService
{
    static bool bIsInitialized;
public:
    static std::string_view GetStaticName() {return "WritableSourceManager";}

    virtual void Initialize() override;
    virtual void Deinitialize() override;
};