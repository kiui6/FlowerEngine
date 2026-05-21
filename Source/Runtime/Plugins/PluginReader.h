#pragma once

#include <Record/IRecordSource.h>
#include <Data/DataView.h>
#include <Data/DataReader.h>

struct is_master_file_t {};
is_master_file_t is_master_file;

class PluginReader : public IRecordSource {
    std::string m_name;

    std::optional<DataView> m_fileView;
    bool m_isMaster;

    std::optional<DataView> m_dependenciesView;

    size_t recordsDataOffset;
    std::optional<DataView> m_recordsView;

    size_t recordsLUTOffset;
    size_t recordsLUTCount;
    std::optional<DataView> m_LUTView;
public:
    PluginReader(std::string_view name) : m_name(name) {}
    PluginReader(is_master_file_t, std::string_view name) : m_name(name), m_isMaster(true) {}

    bool IsMaster() const {return m_isMaster;}
    bool IsValid() const {return m_fileView.has_value() && m_fileView.value();}
    std::string_view GetName() const {return m_name;}

    void InitializeFileView(DataView&& view) {m_fileView = std::move(view);}

    RecordMemory FetchRecordMemory(RecordID id) override {return {};}
    ID32 FetchRecordType(RecordID id) override {return 0;}
    bool HasRecord(RecordID id) override {return false;}
};