#include "DataManager.h"

#include <Platform/Platform.h>
#include <Async/AsyncService.h>
#include <Config/Config.h>

#include "Formats/MasterFile/MasterFile.h"
#include "Formats/PluginFile/PluginFile.h"

#include <SDL3/SDL.h>
#include <Log/Log.h>

#include <filesystem>

bool DataManager::bIsInitialized = RegisterService<DataManager>({Platform::GetStaticName(), AsyncService::GetStaticName(), Config::GetStaticName()});

void DataManager::Initialize()
{

}

void DataManager::Deinitialize()
{
}

const std::filesystem::path& DataManager::GetBasePath()
{
    if(m_basePath.empty()) {
        m_basePath = (std::filesystem::absolute(SDL_GetBasePath()) / "Data").lexically_normal();
    }

    return m_basePath;
}

const std::filesystem::path &DataManager::GetPrefPath()
{
    // TODO: Set pref path namespaces
    if(m_prefPath.empty()) {
        m_prefPath = std::filesystem::absolute(SDL_GetPrefPath("TODO", "TODO")).lexically_normal();
    }

    return m_prefPath;
}

DataView DataManager::OpenDataView(std::string_view relativePath)
{
    std::string path = CanonizePathSandboxed(relativePath);

    auto fileHandle = m_fileHandles.find(path);
    if(fileHandle != m_fileHandles.end()) {
        return DataView(fileHandle->second.file.lock());
    }

    std::shared_ptr<FileBase> file = GetService<Platform>()->OpenFile(path, FileAccess::Read | FileAccess::Binary);

    if(!file->IsOpen()) {
        LOGF(Error, LogData, "Failed to open data view to file: \"%.*s\"", relativePath.length(), relativePath.data());
        return DataView(nullptr);
    }

    FileHandle handle;
    handle.file = file;
    handle.isMapped = false;

    m_fileHandles.emplace(relativePath, std::move(handle));

    return DataView(file);
}

DataView DataManager::MapDataView(std::string_view relativePath)
{
    std::string path = CanonizePathSandboxed(relativePath);

    std::shared_ptr<FileBase> file = GetService<Platform>()->MapFile(path, FileAccess::Read | FileAccess::Binary);

    if(!file->IsOpen()) {
        LOGF(Error, LogData, "Failed to map data view to file: \"%.*s\"", relativePath.length(), relativePath.data());
        return DataView(nullptr);
    }

    FileHandle handle;
    handle.file = file;
    handle.isMapped = true;

    m_fileHandles.emplace(relativePath, std::move(handle));
    
    return DataView(file);
}

DataWriter DataManager::OpenDataWriter(std::string_view relativePath)
{
#ifndef EDITOR
    if(relativePath.starts_with("G")) {
        LOGF(Assert, LogDataWriter, "Illegal file path: \"%.*s\"! Writes to a Game prefix are unsupported.", relativePath.length(), relativePath.data());
        return {};
    }
#endif

    std::string path = CanonizePathSandboxed(relativePath);

    return DataWriter();
}

DirectoryView DataManager::OpenDirectoryView(std::string_view relativePath)
{
    std::string basePath, basePrefix;
    if(relativePath.starts_with("Game:")) {
        basePath = GetBasePath().string();
        basePrefix = "Game:";
    } else if (relativePath.starts_with("Pref:")) {
        basePath = GetPrefPath().string();
        basePrefix = "Pref:";
    } else {
        LOGF(Assert, LogData, "Illegal file path: \"%.*s\"! Path should start from either \"Game:\" or \"Pref:\".", relativePath.length(), relativePath.data());
        return {};
    }

    std::string path = CanonizePathSandboxed(relativePath);

    return DirectoryView(basePath, basePrefix, path);
}

std::string DataManager::CanonizePathSandboxed(std::string_view path)
{
    size_t firstDividerPos = path.find_first_of(":");
    if(firstDividerPos == std::string_view::npos) {
        LOGF(Assert, LogData, "Illegal file path: \"%.*s\"! Path should start from either \"Game:\" or \"Pref:\".", path.length(), path.data());
        return {};
    }

    std::string_view pathPrefix = path.substr(0, firstDividerPos);
    std::string_view relativeOriginalPath = path.substr(firstDividerPos + 1);

    // Remove all leading separators
    while (!relativeOriginalPath.empty() && (relativeOriginalPath.front() == '/' || relativeOriginalPath.front() == '\\')) {
        relativeOriginalPath.remove_prefix(1);
    }

    std::filesystem::path base;

    if(pathPrefix == "Game") {
        base = GetBasePath();
    } else if (pathPrefix == "Pref") {
        base = GetPrefPath();
    } else {
        LOGF(Assert, LogData, "Illegal file path: \"%.*s\"! Path should start from either \"Game:\" or \"Pref:\".", path.length(), path.data());
        return {};
    }

    if(relativeOriginalPath.empty()) {
        return base.string(); 
    }

    std::error_code error;
    std::filesystem::path finalPath = std::filesystem::weakly_canonical(base / relativeOriginalPath, error);
    if(error) {
        LOG(Assert, LogData, "Path canonicalization failed!");
        return {};
    }

    // Check if path is relative to the base.
    std::filesystem::path relativePath = finalPath.lexically_relative(base);

    // If result is empty, they're not relative
    // If relative path starts from '..', path is leaving sandboxed base
    if(relativePath.empty() || *relativePath.begin() == "..") {
        LOGF(Assert, LogData, "Illegal file path: \"%.*s\"! Path is outside of sandbox.", path.length(), path.data());
        return {};
    }

    return finalPath.string();
}