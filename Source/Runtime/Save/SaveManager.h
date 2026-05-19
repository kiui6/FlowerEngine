#pragma once

#include <Service/Service.h>

#include <optional>
#include <memory>
#include <atomic>
#include <shared_mutex>
#include <mutex>

class SaveManager : public IService
{
    static bool bIsInitialized;
public:
    static std::string_view GetStaticName() {return "SaveManager";}

    virtual void Initialize() override;
    virtual void Deinitialize() override;
};