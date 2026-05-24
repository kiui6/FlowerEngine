#pragma once

#include <Record/IRecordSource.h>
#include <Data/DataView.h>
#include <Data/DataReader.h>

#include "Serial/SerialDependency.h"
#include "Serial/SerialLUTEntry.h"
#include "Serial/SerialHeader.h"
#include "Serial/SerialField.h"

struct is_master_plugin_t {};
static constexpr is_master_plugin_t is_master_plugin{};

class PluginReader : public IRecordSource {
    std::string m_name;

    std::optional<DataView> m_fileView;
    bool m_isMaster;

    uint16_t m_dependenciesCount = 0;
    std::unordered_map<uint16_t, uint64_t> m_dependencies;

    std::optional<DataView> m_recordsView;

    size_t m_recordsLUTCount = 0;
    std::optional<DataView> m_LUTView;
public:
    PluginReader(std::string_view name) : m_name(name) {}
    PluginReader(is_master_plugin_t, std::string_view name) : m_name(name), m_isMaster(true) {}

    bool IsMaster() const {return m_isMaster;}
    bool IsValid() const {return m_fileView.has_value() && m_fileView.value();}
    std::string_view GetName() const {return m_name;}

    void InitializeFileView(DataView&& view);

    std::optional<RecordMemory> FetchRecordMemory(RecordID id) override;
    std::optional<ID32> FetchRecordType(RecordID id) override {return {};}
    bool HasRecord(RecordID id) override {return false;}
protected:
    std::optional<SerialLUTEntry> FindRecordLUTEntry(RecordID id); 
    DataView FindRecordFromOffset(size_t offset); 
};