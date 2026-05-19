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
    std::string path1 = CanonizePathSandboxed("Game/");
    std::string path2 = CanonizePathSandboxed("Game/.");
    std::string path3 = CanonizePathSandboxed("Game/./");
    std::string path4 = CanonizePathSandboxed("Game/../");
    std::string path5 = CanonizePathSandboxed("Game/Game.master");
    std::string path6 = CanonizePathSandboxed("Game/Data/../");
    std::string path7 = CanonizePathSandboxed("Game/Data/../Game.master");
    std::string path8 = CanonizePathSandboxed("Game/Data/Game.master");
    std::string path9 = CanonizePathSandboxed("Game/%");
    std::string path10 = CanonizePathSandboxed("Game//");
    std::string path11 = CanonizePathSandboxed("Game\\Data");
    std::string path12 = CanonizePathSandboxed("Game/Data\\Game.master");
}

void DataManager::Deinitialize()
{
}

const std::filesystem::path& DataManager::GetBasePath()
{
    if(m_basePath.empty()) {
        m_basePath = std::filesystem::absolute(SDL_GetBasePath()).lexically_normal();
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
    std::string path;
    if(relativePath.starts_with("Game/")) {
        path = PathBuilder::MakeRelative(GetBasePath().string(), relativePath);
    } else if (relativePath.starts_with("Pref/")) {
        path = PathBuilder::MakeRelative(GetPrefPath().string(), relativePath);
    } else {
        LOGF(Assert, LogData, "Illegal file path: \"%.*s\"! Path should start from either \"Game/\" or \"Pref/\".", relativePath.length(), relativePath.data());
        return DataView(nullptr);
    }

    auto fileHandle = m_fileHandles.find(path);
    if(fileHandle != m_fileHandles.end()) {
        return DataView(fileHandle->second.file.lock());
    }

    std::shared_ptr<FileBase> file = GetService<Platform>()->OpenFile(path, FileAccess::Read | FileAccess::Binary);

    if(!file->IsOpen()) {
        LOGF(Error, LogData, "Failed to open data view to file: \"%s\"", path.c_str());
        return DataView(nullptr);
    }

    FileHandle handle;
    handle.file = file;
    handle.isMapped = false;

    m_fileHandles.emplace(relativePath, handle);

    return DataView(file);
}

DataView DataManager::MapDataView(std::string_view relativePath)
{
    std::string path;
    if(relativePath.starts_with("Game/")) {
        path = PathBuilder::MakeRelative(GetBasePath().string(), relativePath);
    } else if (relativePath.starts_with("Pref/")) {
        path = PathBuilder::MakeRelative(GetPrefPath().string(), relativePath);
    } else {
        LOGF(Assert, LogData, "Illegal file path: \"%.*s\"! Path should start from either \"Game/\" or \"Pref/\".", relativePath.length(), relativePath.data());
        return DataView(nullptr);
    }

    std::shared_ptr<FileBase> file = GetService<Platform>()->MapFile(path, FileAccess::Read | FileAccess::Binary);

    if(!file->IsOpen()) {
        return DataView(nullptr);
    }

    FileHandle handle;
    handle.file = file;
    handle.isMapped = true;

    m_fileHandles.emplace(relativePath, handle);
    
    return DataView(file);
}

std::string DataManager::CanonizePathSandboxed(std::string_view path)
{
    size_t firstDividerPos = path.find_first_of("/");
    if(firstDividerPos == std::string_view::npos) {
        LOGF(Assert, LogData, "Illegal file path: \"%.*s\"! Path should start from either \"Game/\" or \"Pref/\".", path.length(), path.data());
        return {};
    }

    std::string_view pathPrefix = path.substr(0, firstDividerPos + 1);
    std::string_view relativeOriginalPath = path.substr(firstDividerPos + 1);   


    // Remove all leading separators
    while (!relativeOriginalPath.empty() && relativeOriginalPath.front() == '/') {
        relativeOriginalPath.remove_prefix(1);
    }

    std::filesystem::path base;

    if(pathPrefix == "Game/") {
        base = GetBasePath();
    } else if (pathPrefix == "Pref/") {
        base = GetPrefPath();
    } else {
        LOGF(Assert, LogData, "Illegal file path: \"%.*s\"! Path should start from either \"Game/\" or \"Pref/\".", path.length(), path.data());
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

    std::filesystem::path relativePath = finalPath.lexically_relative(base);

    if(relativePath.empty() || *relativePath.begin() == "..") {
        LOGF(Assert, LogData, "Illegal file path: \"%.*s\"! Path is outside of sandbox.", path.length(), path.data());
        return {};
    }

    return finalPath.string();
}
