#include "PluginManager.h"

#include <Data/DataManager.h>
#include <Record/RecordLibrary.h>
#include <Config/Config.h>

#include <Log/Log.h>

#include <set>

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

    // Load Master file
    std::string_view masterFileName = config->GetNamespace("Game").GetString("Master.Filename", "Game.master");

    m_loadedPlugins.emplace_back(PluginReader(is_master_plugin, masterFileName));

    // Read load order config and fill loaded plugins list
    // Loaded plugins list contains PluginReaders without path prefix, because storing it will be redundant.
    // Plugins can't be contained inside of a Pref: prefix.
    DataView loadOrderConfigView = dataMgr->OpenDataView("Pref:plugins.txt");
    if(loadOrderConfigView) {
        TextualDataReader loadOrderConfigReader(loadOrderConfigView);

        std::optional<std::string> loadOrderConfigLine = loadOrderConfigReader.ReadLine();
        while(loadOrderConfigLine.has_value()) {
            m_loadedPlugins.push_back(PluginReader(loadOrderConfigLine.value()));

            loadOrderConfigLine = loadOrderConfigReader.ReadLine();
        }
    }
    // Just in case check for load order vector to be smaller than size of uint16_t
    if(m_loadedPlugins.size() >= static_cast<uint16_t>(-1)) {
        LOGF(Fatal, LogPlugin, "Too many plugins enabled, maximum supported is %u", static_cast<uint16_t>(-1));
        return false;
    }
    
    // TODO: Load order resolution

    // Once all plugin files and load orders are resolved, signal files as record sources to the RecordLibrary
    RecordLibrary* recLib = GetService<RecordLibrary>();
    for(size_t pluginIndex = 0; pluginIndex < m_loadedPlugins.size(); pluginIndex++) {
        PluginReader* plugin = &m_loadedPlugins[pluginIndex];
        
        // Initialize file with a data view
        std::string pluginFilePath = "Game:" + std::string(plugin->GetName());
        DataView view = dataMgr->MapDataView(pluginFilePath);
        if(view) {
            plugin->InitializeFileView(std::move(view));
        } else {
            LOG(Error, LogPlugin, "Failed to map data view into a plugin file.");
        }

        // Check if plugin was opened and parsed successfully
        // TODO: And if the pluginID is unique
        if(plugin->IsValid()) {
            if(!recLib->AddRecordSource(plugin)) {
                LOG(Error, LogPlugin, "Failed to add record source at index.");
                continue;
            }
            LOGF(Log, LogPlugin, "Loaded plugin: %.*s", plugin->GetName().length(), plugin->GetName().data());
        } else {
            LOG(Error, LogPlugin, "Tried adding invalid plugin.");
            continue;
        }
    }

    return true;
}
