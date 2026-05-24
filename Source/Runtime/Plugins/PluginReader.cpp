#include "PluginReader.h"

#include <Log/Log.h>

#include <vector>

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

    auto rec = FetchRecordMemory(3);
}

std::optional<RecordMemory> PluginReader::FetchRecordMemory(RecordID id)
{
    std::optional<SerialLUTEntry> lutEntry = FindRecordLUTEntry(id);
    if(!lutEntry) {
        return {};
    }

    DataView recordFieldsView = FindRecordFromOffset(lutEntry->offset);
    DataReader recordFieldsReader(recordFieldsView);

    RecordMemory memory;
    memory.SetRecordID(id);
    memory.SetRecordType(lutEntry->type); 

    // If record is marked as deleted in the Look-up Table, we can ignore reading any fields
    if(lutEntry->flags & PLUGIN_SERIAL_RECORD_FLAG_DELETED) {
        memory.SetDeleted(true);
        return memory;
    }    

    for(uint16_t i = 0; i < lutEntry->fieldsCount; i++) {
        std::optional<SerialField> fieldHeader = recordFieldsReader.Read<SerialField>();
        if(!fieldHeader) {
            LOGF(Assert, LogPluginReader, "Expected field header, but met unexpected EOF.");
            return {};
        }

        std::optional<std::byte*> fieldData = recordFieldsReader.ReadBytes(fieldHeader->dataSize);
        if(!fieldData) {
            LOGF(Assert, LogPluginReader, "Expected field data, but met unexpected EOF.");
            return {};
        }

        RecordFieldMemory fieldMem;
        fieldMem.data.assign(*fieldData, *fieldData + fieldHeader->dataSize);
        
        memory.AddField(fieldHeader->id, std::move(fieldMem));
    }

    return memory;
}

std::optional<SerialLUTEntry> PluginReader::FindRecordLUTEntry(RecordID id)
{
    size_t entryOffset = 0;
    DataReader reader(*m_LUTView);

    size_t left = 0, right = m_recordsLUTCount;
    while(left <= right) {
        // Division result should be guaranteed to be truncated towards zero
        size_t mid = left + (right - left) / 2;

        reader.Advance(mid * sizeof(SerialLUTEntry));
        RecordID entryID = *reader.Read<RecordID>(false);

        if (entryID == id) return reader.Read<SerialLUTEntry>(false);
        else if (entryID < id) left = mid + 1;
        else right = mid - 1;
    }
    return {};
}

DataView PluginReader::FindRecordFromOffset(size_t offset)
{
    return m_recordsView->MakeSubView(offset, m_recordsView->size() - offset);
}
