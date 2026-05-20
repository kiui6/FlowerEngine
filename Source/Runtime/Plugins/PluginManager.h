#pragma once

#include <Service/Service.h>

#include <optional>
#include <memory>
#include <atomic>
#include <shared_mutex>
#include <mutex>

#include "PluginFile.h"

class PluginManager : public IService
{
    static bool bIsInitialized;

    std::vector<PluginFile> m_plugins;
    std::vector<size_t> m_loadOrder;
public:
    static std::string_view GetStaticName() {return "PluginManager";}

    virtual void Initialize() override;
    virtual void Deinitialize() override;
};