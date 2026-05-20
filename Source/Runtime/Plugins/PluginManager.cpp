#include "PluginManager.h"

#include <Data/DataManager.h>
#include <Record/RecordLibrary.h>

#include <Log/Log.h>

bool PluginManager::bIsInitialized = RegisterService<PluginManager>({DataManager::GetStaticName(), RecordLibrary::GetStaticName()});

void PluginManager::Initialize()
{
    DataManager* dataMgr = GetService<DataManager>();

    DirectoryView rootView = dataMgr->OpenDirectoryView("Game:");

    for(const DirectoryView& childView : rootView.GetEntries()) {
        if(childView.HasExtension(".master") || childView.HasExtension(".plugin")) {
            LOGF(Log, LogPlugin, "Detected plugin at \"%s\"", childView.GetPath().c_str());
            m_plugins.emplace_back(PluginFile(childView.GetPath()));
        }
    }
}

void PluginManager::Deinitialize()
{
}
