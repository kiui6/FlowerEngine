#include "PluginReader.h"

#include <Log/Log.h>
#include <Debug/Tracer/Tracer.h>

#include <vector>

void PluginReader::InitializeFileView(DataView &&view)
{
    m_fileView = std::move(view);

    if(!m_fileView) {
        LOG(Assert, LogPluginReader, "Plugin reader got invalid data view");
        return;
    }

    DataReader fileReader(m_fileView);

    SerialHeader header;
    if(!fileReader.Read<SerialHeader>(header)) {
        LOG(Assert, LogPluginReader, "Plugin reader couldn't read plugin's header");
        return;
    }

    m_uniqueID = header.pluginID;

    m_dependenciesCount = header.dependencyCount; 
    if(m_dependenciesCount) {
        DataView dependenciesView = m_fileView.MakeSubView(header.dependenciesOffset, m_dependenciesCount * sizeof(SerialDependency));
        DataReader dependenciesReader(dependenciesView);

        SerialDependency dependency;
        while(dependenciesReader.Read<SerialDependency>(dependency)) {
            m_dependencies.emplace(dependency.pluginOrderID, dependency.sourceID);
        }
    }

    m_recordsView = m_fileView.MakeSubView(header.recordsBlobOffset, header.recordsBlobSize);
    
    m_recordsLUTCount = header.recordsLutCount;
    m_LUTView = m_fileView.MakeSubView(header.recordsLutOffset, header.recordsLutCount * sizeof(SerialLUTEntry));
}

bool PluginReader::FetchRecordInfo(RecordID id, RecordInformation &result)
{
    SerialLUTEntry lutEntry;
    if(!FindRecordLUTEntry(id, lutEntry)) {
        return false;
    }

    result.fieldsCount = lutEntry.fieldsCount;
    result.isDeleted = lutEntry.flags & SerialRecordFlags::Deleted;
    result.type = lutEntry.type;

    return true;
}

bool PluginReader::PopulateRecordFieldObjectBase(RecordID id, RecordObject &result)
{
    SerialLUTEntry lutEntry;
    if(!FindRecordLUTEntry(id, lutEntry)) {
        return false;
    }

    DataView recordFieldsView = FindRecordFromOffset(lutEntry.offset);
    DataReader recordFieldsReader(recordFieldsView);

    result.SetID(id);
    result.SetType(lutEntry.type); 

    // If record is marked as deleted in the Look-up Table, we can ignore reading any fields
    if(lutEntry.flags & SerialRecordFlags::Deleted) {
        result.SetDeleted(true);
        return true;
    }

    RecordFieldObject& fieldObject = result.CreateFieldObject();

    SerialField fieldHeader;
    for(uint16_t i = 0; i < lutEntry.fieldsCount; i++) {
        if(!recordFieldsReader.Read<SerialField>(fieldHeader)) {
            LOG(Assert, LogPluginReader, "Expected field header, but met unexpected EOF.");
            return false;
        }

        RecordFieldObject::NodeWrapper node = fieldObject.CreateField(fieldHeader.id, fieldHeader.type);

        if(!SerialFormatBaseReader::PopulateFieldFromSerial(recordFieldsReader, fieldHeader.type, node)) {
            return false;
        }
    }

    return true;
}

bool PluginReader::PopulateRecordFieldObjectDelta(RecordID id, RecordObject &result)
{
    return false;
}

bool PluginReader::FindRecordLUTEntry(RecordID id, SerialLUTEntry& result)
{
    size_t entryOffset = 0;
    DataReader reader(m_LUTView);

    int64_t left = 0, right = m_recordsLUTCount - 1;
    RecordID entryID;
    while(left <= right) {
        // Division result should be guaranteed to be truncated towards zero
        size_t mid = left + (right - left) / 2;

        reader.SetOffset(mid * sizeof(SerialLUTEntry));
        reader.Read<RecordID>(entryID, false);

        if (entryID == id) {
            return reader.Read<SerialLUTEntry>(result, false);
        }
        else if (entryID < id) { 
            left = mid + 1;
        }
        else { 
            right = mid - 1;
        }
    }
    return false;
}

DataView PluginReader::FindRecordFromOffset(size_t offset)
{
    return m_recordsView.MakeSubView(offset, m_recordsView.size() - offset);
}

bool SerialFormatBaseReader::ParseString(DataReader &reader, RecordFieldObject::NodeWrapper &result)
{
    uint32_t strSize;
    if(!reader.Read<uint32_t>(strSize)) {
            LOG(Assert, LogPluginReader, "Expected string size, but met unexpected EOF.");
            return false;
    }

    result.SetSize(strSize);
    result.SetString(reinterpret_cast<const char*>(reader.GetData()));
    reader.Advance(strSize);

    return true;
}
