#pragma once

#include <Service/Service.h>

#include <optional>
#include <memory>
#include <atomic>
#include <shared_mutex>
#include <mutex>

#include "Formats/MasterFile/MasterFile.h"
#include "Formats/PluginFile/PluginFile.h"

#include <Platform/Platform.h>
#include <Utility/PathBuilder.h>

class DataManager : public IService
{
    static bool bIsInitialized;

    mutable std::shared_mutex m_mtx;

    std::vector<MasterFile> m_masters;
    //TODO: std::vector<PluginFile> m_plugins;

    std::string m_basePath, m_savePath;
public:
    static std::string_view GetStaticName() {return "DataManager";}

    virtual void Initialize() override;
    virtual void Deinitialize() override;

    File OpenAssetFile(std::string relativePath);
};