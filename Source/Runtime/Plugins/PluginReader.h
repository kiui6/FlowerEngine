#pragma once

#include <unordered_map>

#include <Record/IRecordSource.h>
#include <Data/FileView.h>
#include <Data/DataReader.h>

#include <Log/Log.h>

#include "Serial/SerialDependency.h"
#include "Serial/SerialLUTEntry.h"
#include "Serial/SerialHeader.h"
#include "Serial/SerialField.h"

class SerialFormatBaseReader {
    using FieldParser = bool (*)(DataReader&, RecordFieldObject::NodeWrapper&);
public:
    static inline bool PopulateFieldFromSerial(DataReader& reader, FieldNodeType type, RecordFieldObject::NodeWrapper& result) {
        switch(type) {
            case FieldNodeType::Integer: return ParseTrivial<int32_t>(reader, result);
            case FieldNodeType::Unsigned: return ParseTrivial<uint32_t>(reader, result);
            case FieldNodeType::Float: return ParseTrivial<float>(reader, result);
            case FieldNodeType::Double: return ParseTrivial<double>(reader, result);
            case FieldNodeType::String: return ParseString(reader, result);
            case FieldNodeType::Map: return ParseMap(reader, result);
            case FieldNodeType::List: return ParseList(reader, result);
            default: 
                LOG(Assert, LogSerialFormatBaseReader, "Unsupported field element type!"); 
                return false;
        }
    }
private:
    template <typename T>
    static bool ParseTrivial(DataReader& reader, RecordFieldObject::NodeWrapper& result) {
        uint32_t dataSize = sizeof(T);

        if(!reader.ReadBytes(dataSize, &result.GetNode()->data)) {
            LOG(Assert, LogPluginReader, "Expected field data, but met unexpected EOF.");
            return false;
        }

        return true;
    }

    static bool ParseString(DataReader& reader, RecordFieldObject::NodeWrapper& result);
    static bool ParseMap(DataReader& reader, RecordFieldObject::NodeWrapper& result);
    static bool ParseList(DataReader& reader, RecordFieldObject::NodeWrapper& result);
};

struct is_master_plugin_t {};
static constexpr is_master_plugin_t is_master_plugin{};

class PluginReader : public IRecordSource {
    std::string m_name;
    uint64_t m_uniqueID;

    FileView m_fileView{};
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

    void InitializeFileView(FileView&& view);

    // IRecordSource Interface
    virtual bool ResolveUniquePluginID(uint64_t uniqueID, uint16_t& relativeID) override {return false;}
    virtual uint64_t GetUniquePluginID() override {return m_uniqueID;}
    virtual bool FetchRecordInfo(RecordID id, RecordInformation& result) override;
    bool PopulateRecordFieldObjectBase(RecordID id, RecordObject& result) override;
    bool PopulateRecordFieldObjectDelta(RecordID id, RecordObject& result) override;
    std::optional<ID32> FetchRecordType(RecordID id) override {return {};}
    bool HasRecord(RecordID id) override {return false;}
protected:
    bool FindRecordLUTEntry(RecordID id, SerialLUTEntry& result);
    DataView FindRecordFromOffset(size_t offset);
};