#pragma once

#include <Service/Service.h>

#include <optional>
#include <memory>
#include <atomic>
#include <shared_mutex>
#include <mutex>
#include <filesystem>

#include "DataWriter.h"
#include "DataView.h"
#include "FileView.h"
#include "FileBuffer.h"
#include "DirectoryView.h"

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

    std::unordered_map<std::string, FileHandle> m_fileHandles;

    std::filesystem::path m_basePath, m_prefPath;
public:
    static std::string_view GetStaticName() {return "DataManager";}

    virtual void Initialize() override;
    virtual void Deinitialize() override;

    FileView OpenFileRead(std::string_view relativePath);
    FileView MapFileRead(std::string_view relativePath);

    /*
     * Opens a file and provides a buffer for it.
     *
     * @warning
     * Calling with "Game/" prefix in Game build will fail, in Game build writing is only allowed to "Pref/" prefix.
     */
    FileBuffer OpenFileWrite(std::string_view relativePath);
    FileBuffer MapFileWrite(std::string_view relativePath);

    DirectoryView OpenDirectoryView(std::string_view relativePath);
protected:
    const std::filesystem::path& GetBasePath();
    const std::filesystem::path& GetPrefPath();

    std::string CanonicalizePathSandboxed(std::string_view path);
};