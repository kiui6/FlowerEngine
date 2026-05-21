#include "PluginManager.h"

#include <Data/DataManager.h>
#include <Record/RecordLibrary.h>
#include <Config/Config.h>

#include <Log/Log.h>

#include <set>

bool PluginManager::bIsInitialized = RegisterService<PluginManager>({Config::GetStaticName(), DataManager::GetStaticName(), RecordLibrary::GetStaticName()});

void PluginManager::Initialize()
{
    DataManager* dataMgr = GetService<DataManager>();
    Config* config = GetService<Config>();

    // Load Master file
    std::string_view masterFileName = config->GetNamespace("Game").GetString("Master.Filename", "Game.master");

    m_loadedPlugins.emplace_back(PluginReader(is_master_file, masterFileName));

    // Read load order config
    std::vector<std::string> loadOrder;

    DataView loadOrderConfigView = dataMgr->OpenDataView("Pref:plugins.txt");
    if(loadOrderConfigView) {
        TextualDataReader loadOrderConfigReader(loadOrderConfigView);

        std::optional<std::string_view> loadOrderConfigLine = loadOrderConfigReader.ReadNextLine();
        while(loadOrderConfigLine.has_value()) {
            loadOrder.push_back(std::string(loadOrderConfigLine.value()));

            loadOrderConfigLine = loadOrderConfigReader.ReadNextLine();
        }
    }

    // Collect plugins available for load
    // We assemble the list of available plugins to allow runtime load order setup.
    DirectoryView rootView = dataMgr->OpenDirectoryView("Game:");
    for(const DirectoryView& childView : rootView.GetEntries()) {
        if(childView.HasExtension(".plugin")) {
            LOGF(Log, LogPlugin, "Detected plugin at \"%s\"", childView.GetPath().c_str());
            m_plugins.emplace_back(PluginReader(childView.GetName()));
        }
    }

    // Add plugins into the load order according to the list.
    for(const std::string& loadOrderPluginName : loadOrder) {
        for(size_t pluginFileIndex = 0; pluginFileIndex < m_plugins.size(); pluginFileIndex++) {
            if(m_plugins[pluginFileIndex].GetName() == loadOrderPluginName) {
                m_loadOrder.push_back(pluginFileIndex);
            }
        }
    }
    
    // Just in case check for load order vector to be smaller than size of uint16_t
    if(m_loadOrder.size() >= static_cast<uint16_t>(-1)) {
        LOGF(Fatal, LogPlugin, "Too many plugins enabled, maximum supported is %u", static_cast<uint16_t>(-1));
        return;
    }

    // Once all plugin files and load orders are resolved, signal files as record sources to the RecordLibrary
    RecordLibrary* recLib = GetService<RecordLibrary>();
    for(size_t loadOrderIndex = 0; loadOrderIndex < m_loadOrder.size(); loadOrderIndex++) {
        PluginReader* plugin = &m_plugins[m_loadOrder[loadOrderIndex]];
        
        // Initialize file with a data view
        std::string PluginFilePath = "Game:" + std::string(plugin->GetName());
        DataView view = dataMgr->MapDataView(PluginFilePath);
        if(!view) {
            LOG(Fatal, LogPlugin, "Failed to map data view into a plugin file.");
            return;
        }
        plugin->InitializeFileView(std::move(view));

        // Check if plugin was opened and parsed successfully
        if(plugin->IsValid()) {
            if(!recLib->AddRecordSource(plugin)) {
                LOG(Error, LogPlugin, "Failed to add record source at index.");
                return;
            }
        } else {
            LOG(Error, LogPlugin, "Tried adding invalid plugin.");
            return;
        }
    }
}

void PluginManager::Deinitialize()
{
}
