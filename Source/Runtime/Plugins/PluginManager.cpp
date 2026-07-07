#include "PluginManager.h"

#include <Data/DataManager.h>
#include <Record/RecordLibrary.h>
#include <Config/Config.h>

#include <Log/Log.h>

#include <set>
#include <vector>

bool PluginManager::bIsInitialized = RegisterService<PluginManager>({Config::GetStaticName(), DataManager::GetStaticName(), RecordLibrary::GetStaticName()});

void PluginManager::Initialize()
{
}

void PluginManager::Deinitialize()
{
}

// TODO: Implement proper PluginOrderError returns
bool PluginManager::LoadDefaultPlugins(PluginOrderError &error)
{
    DataManager* dataMgr = GetService<DataManager>();
    Config* config = GetService<Config>();

    std::vector<std::string_view> loadSourceCandidates{};

    // Load Master file
    std::string_view masterFileName = config->GetNamespace("Game").GetString("Master.Filename", "Game.master");
    loadSourceCandidates.push_back(masterFileName);

    // Read load order config and fill loaded plugins list
    // Loaded plugins list contains PluginReaders without path prefix, because storing it will be redundant.
    // Plugins can't be contained inside of a Pref: prefix.
    FileView loadOrderConfigView = dataMgr->OpenFileRead("Pref:plugins.txt");
    if(loadOrderConfigView) {
        DataView loadOrderConfigDataView = loadOrderConfigView.MakeView();
        TextualDataReader loadOrderConfigReader(loadOrderConfigDataView);

        std::optional<std::string> loadOrderConfigLine = loadOrderConfigReader.ReadLine();
        while(loadOrderConfigLine.has_value()) {
            loadSourceCandidates.push_back(loadOrderConfigLine.value());

            loadOrderConfigLine = loadOrderConfigReader.ReadLine();
        }
    }
    // Just in case check for load order vector to be smaller than size of uint16_t
    if(loadSourceCandidates.size() >= static_cast<uint16_t>(-1)) {
        LOGF(Fatal, LogPlugin, "Too many plugins enabled, maximum supported is %u", static_cast<uint16_t>(-1));
        return false;
    }
    
    // TODO: Load order resolution

    // Once all plugin files and load orders are resolved, signal files as record sources to the RecordLibrary
    RecordLibrary* recLib = GetService<RecordLibrary>();
    for(size_t pluginIndex = 0; pluginIndex < loadSourceCandidates.size(); pluginIndex++) {
        std::string_view pluginPath = loadSourceCandidates[pluginIndex];
        
        // Initialize file with a data view
        std::string pluginFilePath = "Game:" + std::string(pluginPath);
        FileView view = dataMgr->MapFileRead(pluginFilePath);
        if(!view) {
            LOG(Error, LogPlugin, "Failed to map data view into a plugin file.");
        }

        DataView dataView = view.MakeView();
        SourceReader reader = SourceReader(std::move(dataView));

        // Check if plugin was opened and parsed successfully
        // TODO: And if the pluginID is unique
        if(!reader.IsValid()) {
            LOG(Error, LogPlugin, "Tried adding invalid plugin.");
            continue;
        }
        
        LoadedPluginHandle& loadedPluginHandle = m_loadedPlugins.emplace_back(std::move(view), std::move(reader));

        if(!recLib->AddRecordSource(&loadedPluginHandle.sourceReader)) {
            LOG(Error, LogPlugin, "Failed to add record source at index.");
            continue;
        }
        LOGF(Log, LogPlugin, "Loaded plugin: %.*s", pluginFilePath.length(), pluginFilePath.data());

    }

    return true;
}
