#include "PluginManager.h"

#include <Data/DataManager.h>
#include <Record/RecordLibrary.h>

#include <Log/Log.h>

bool PluginManager::bIsInitialized = RegisterService<PluginManager>({DataManager::GetStaticName(), RecordLibrary::GetStaticName()});

void PluginManager::Initialize()
{
    DataManager* dataMgr = GetService<DataManager>();

    // TODO: Read load order config
    DataView loadOrderConfig = dataMgr->OpenDataView("Pref:plugins.txt");

    // Collect plugins available for load
    DirectoryView rootView = dataMgr->OpenDirectoryView("Game:");
    for(const DirectoryView& childView : rootView.GetEntries()) {
        if(childView.HasExtension(".master")) {
            LOGF(Log, LogPlugin, "Detected master at \"%s\"", childView.GetPath().c_str());
            m_plugins.emplace_back(PluginFile(is_master_file, childView.GetPath()));
        } else if(childView.HasExtension(".plugin")) {
            LOGF(Log, LogPlugin, "Detected plugin at \"%s\"", childView.GetPath().c_str());
            m_plugins.emplace_back(PluginFile(childView.GetPath()));
        }
    }

    // Instantiate plugin files
    size_t pluginFileIndex = 0;
    for(PluginFile& plugin : m_plugins) {
        if(plugin.IsMaster()) {
            // TODO: There should be only one main master, and optional master files for DLCs. Check dependencies to ensure load order
            DataView view = dataMgr->MapDataView(plugin.GetPath());
            if(!view) {
                LOG(Error, LogPlugin, "Failed to map data view into a master file.");
                return;
            }
            plugin.InitializeFileView(std::move(view));
            m_loadOrder.push_back(pluginFileIndex);
        }
        pluginFileIndex++;
    }
    
    // Just in case check for load order vector to be smaller than size of uint16_t
    if(m_loadOrder.size() >= static_cast<uint16_t>(-1)) {
        LOGF(Fatal, LogPlugin, "Too many plugins enabled, maximum supported is %u", static_cast<uint16_t>(-1));
        return;
    }

    // Once all plugin files and load orders are resolved, signal files as record sources to the RecordLibrary
    RecordLibrary* recLib = GetService<RecordLibrary>();
    for(size_t loadOrderIndex = 0; loadOrderIndex < m_loadOrder.size(); loadOrderIndex++) {
        PluginFile* plugin = &m_plugins[m_loadOrder[loadOrderIndex]];
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
