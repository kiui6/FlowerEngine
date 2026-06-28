#pragma once

#include <unordered_map>

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
    uint64_t m_uniqueID;

    DataView m_fileView{};
    bool m_isMaster;

    uint16_t m_dependenciesCount = 0;
    std::unordered_map<uint16_t, uint64_t> m_dependencies;

    DataView m_recordsView{};

    size_t m_recordsLUTCount = 0;
    DataView m_LUTView{};
public:
    PluginReader(std::string_view name) : m_name(name) {}
    PluginReader(is_master_plugin_t, std::string_view name) : m_name(name), m_isMaster(true) {}

    bool IsMaster() const {return m_isMaster;}
    bool IsValid() const {return m_fileView;}
    std::string_view GetName() const {return m_name;}

    void InitializeFileView(DataView&& view);

    // IRecordSource Interface
    virtual bool ResolveUniquePluginID(uint64_t uniqueID, uint16_t& relativeID) override {return false;}
    virtual uint64_t GetUniquePluginID() override {return m_uniqueID;}
    virtual bool FetchRecordInfo(RecordID id, RecordInformation& result) override;
    bool PopulateRecordFieldObject(RecordID id, RecordObject& result) override;
    std::optional<ID32> FetchRecordType(RecordID id) override {return {};}
    bool HasRecord(RecordID id) override {return false;}
protected:
    bool FindRecordLUTEntry(RecordID id, SerialLUTEntry& result);
    DataView FindRecordFromOffset(size_t offset);

    uint8_t GetFixedFieldSizeFromType(FieldNodeType type);
};