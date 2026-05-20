#pragma once

#include <Record/IRecordSource.h>
#include <Data/DataView.h>
#include <Data/DataReader.h>

class PluginFile : public IRecordSource {
    std::string m_path;

    std::optional<DataView> m_fileView;
    bool m_isMaster;

    size_t recordsDataOffset;
    std::optional<DataView> m_recordsView;

    size_t recordsLUTOffset;
    size_t recordsLUTCount;
    std::optional<DataView> m_LUTView;
public:
    PluginFile(std::string_view path) : m_path(path) {}

    RecordMemory FetchRecordMemory(RecordID id) override {return {};}
    ID32 FetchRecordType(RecordID id) override {return 0;}
    bool HasRecord(RecordID id) override {return false;}
};