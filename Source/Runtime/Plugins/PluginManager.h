#pragma once

#include <Service/Service.h>

#include <optional>
#include <memory>
#include <atomic>
#include <shared_mutex>
#include <mutex>

#include "PluginReader.h"

#ifdef EDITOR
#   include "PluginWriter.h"
#endif

class PluginManager : public IService
{
    static bool bIsInitialized;

    // Contains loaded plugins for this session
    // Should not be changed after initialization, unless reinitialization is called (i.e. load order is modified)
    std::vector<PluginReader> m_loadedPlugins;
public:
    static std::string_view GetStaticName() {return "PluginManager";}

    virtual void Initialize() override;
    virtual void Deinitialize() override;
};