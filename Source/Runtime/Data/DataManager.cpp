#include "DataManager.h"

#include <Platform/Platform.h>
#include <Async/AsyncService.h>
#include <Config/ConfigManager.h>

#include "Formats/MasterFile/MasterFile.h"
#include "Formats/PluginFile/PluginFile.h"

#include <SDL3/SDL.h>
#include <Log/Log.h>

bool DataManager::bIsInitialized = RegisterService<DataManager>({Platform::GetStaticName(), AsyncService::GetStaticName(), ConfigManager::GetStaticName()});

void DataManager::Initialize()
{
    m_basePath = SDL_GetBasePath();
    // TODO: PrefPath
    m_prefPath = SDL_GetPrefPath("TODO", "TODO");
}

void DataManager::Deinitialize()
{
}

DataView DataManager::OpenDataView(std::string_view relativePath)
{
    const std::string path = PathBuilder::MakeAbsolute(m_basePath, relativePath);

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
    std::shared_ptr<FileBase> file = GetService<Platform>()->MapFile(PathBuilder::MakeAbsolute(m_basePath, relativePath), FileAccess::Read | FileAccess::Binary);

    if(!file->IsOpen()) {
        return DataView(nullptr);
    }

    FileHandle handle;
    handle.file = file;
    handle.isMapped = true;

    m_fileHandles.emplace(relativePath, handle);
    
    return DataView(file);
}
