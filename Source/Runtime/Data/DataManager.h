#pragma once

#include <Service/Service.h>

#include <optional>
#include <memory>
#include <atomic>
#include <shared_mutex>
#include <mutex>

#include "DataView.h"

#include "Formats/MasterFile/MasterFile.h"
#include "Formats/PluginFile/PluginFile.h"

#include <Platform/Platform.h>
#include <Utility/PathBuilder.h>

class DataManager : public IService
{
    struct FileHandle {
        std::weak_ptr<FileBase> file;
        bool isMapped = false;
    };

    static bool bIsInitialized;

    mutable std::shared_mutex m_mtx;

    std::vector<MasterFile> m_masters;
    //TODO: std::vector<PluginFile> m_plugins;

    std::unordered_map<std::string, FileHandle> m_fileHandles;

    std::string m_basePath, m_prefPath;
public:
    static std::string_view GetStaticName() {return "DataManager";}

    virtual void Initialize() override;
    virtual void Deinitialize() override;

    DataView OpenDataView(std::string_view relativePath);
    DataView MapDataView(std::string_view relativePath);
};