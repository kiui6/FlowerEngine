#pragma once

#include <Service/Service.h>

#include <optional>
#include <memory>
#include <atomic>
#include <shared_mutex>
#include <mutex>

#include <SourceFormat/SourceReader.h>
#include <Data/FileView.h>

#include "PluginOrderError.h"

#ifdef EDITOR
#   include <SourceFormat/SourceWriter.h>
#endif

class PluginManager : public IService
{
    static bool bIsInitialized;

    struct LoadedPluginHandle {
        FileView sourceFile;
        SourceReader sourceReader;
    };

    // Contains loaded plugins for this session
    // Should not be changed after initialization, unless reinitialization is called (i.e. load order is modified)
    std::vector<LoadedPluginHandle> m_loadedPlugins;
public:
    static std::string_view GetStaticName() {return "PluginManager";}

    virtual void Initialize() override;
    virtual void Deinitialize() override;

    std::optional<PluginOrderError> LoadDefaultPlugins();
};