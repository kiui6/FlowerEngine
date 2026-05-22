#include "PluginReader.h"

#include <Log/Log.h>

void PluginReader::InitializeFileView(DataView &&view)
{
    m_fileView = std::move(view);

    if(!m_fileView) {
        LOGF(Assert, LogPluginReader, "Plugin reader got invalid data view");
        return;
    }

    DataReader fileReader(*m_fileView);

    std::optional<SerialHeader> header = fileReader.Read<SerialHeader>();
    if(!header) {
        LOGF(Assert, LogPluginReader, "Plugin reader couldn't read plugin's header");
        return;
    }

    m_dependenciesCount = header->dependencyCount; 
    if(m_dependenciesCount) {
        DataView dependenciesView = m_fileView->MakeSubView(header->dependenciesOffset, m_dependenciesCount * sizeof(SerialDependency));
        DataReader dependenciesReader(dependenciesView);

        std::optional<SerialDependency> dependency = dependenciesReader.Read<SerialDependency>();
        while(dependency.has_value()) {
            m_dependencies.emplace(dependency->prefixIndex, dependency->dependencyID);

            dependency = dependenciesReader.Read<SerialDependency>();
        }
    }

    m_recordsView = m_fileView->MakeSubView(header->recordsBlobOffset, header->recordsBlobSize);
    
    m_recordsLUTCount = header->recordsLutCount;
    m_LUTView = m_fileView->MakeSubView(header->recordsLutOffset, header->recordsLutCount * sizeof(SerialLUTEntry));
}
