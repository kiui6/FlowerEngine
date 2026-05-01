#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <array>
#include <concepts>

#include <Utility/Record.h>
#include <GarbageCollector/ReferenceCounter.h>

#include "Field.h"

#include "FieldContainers/StringField.h"

class World;
class Actor;

using RecordID = uint64_t;

constexpr RecordID INVALID_RECORD = 0;
constexpr RecordID PLAYER_RECORD = 1;
constexpr RecordID GBLACKBOARD_RECORD = 2;

/*
 * Loaded Records may contain these flags, that define their runtime behaviour
 */
enum class RecordFlags : uint8_t {
    // This Record is created only for runtime use, do not save on disk. Destroy permamently once parent is unloaded
    Transient = (1 << 0),
    // Unload on next tick. If set, should not be referenced or used by anything
    Unload = (1<<1),
    // This Record was changed and needs changes to be saved (Save System or Asset System) 
    Dirty = (1<<2),
    // Marks this Record undestructable by GC
    BypassGC = (1<<3),
};

/*
 * Records are the main persistent containers inside the engine
 * Each Record has it's unique 64bit ID, that must be consistent between all inner systems
 * Alongside Name string, which is used by Editor and debug features to introduce user friendly naming
 * 
 * Automated behaviours of Record can be changed by setting flags
 * 
 * Record ID 0 is reserved for None; 
 */
class Record : public ReferenceCounter
{
    virtual void RequestSelfDestruction() override;
protected:
    uint16_t m_flags = 0;
    RecordID m_id = INVALID_RECORD;
    uint32_t m_type;
public:
    Field<StringField> EditorID = {FIELDID(EDID), "Untitled"};

    Record() {}
    Record(RecordID id) {m_id = id;}

    Record(const Record&) = delete;
    Record& operator=(const Record&) = delete;
    Record(Record&&) = delete;
    Record& operator=(Record&&) = delete;

    virtual ~Record(){}

    static uint32_t StaticType() {return MakeRecordType("UNKN");}
    uint32_t& GetType() {return m_type;}

    void Rename(const std::string& newName);
    // TODO: Maybe make this private? It can cause big issues
    void SetID(RecordID newID) {m_id = newID;}
    void SetType(uint32_t newType) {m_type = newType;}

    inline RecordID GetID() const {return m_id;}

    virtual std::vector<FieldBase*> GetFields() {
        std::vector<FieldBase*> fields;
        fields.push_back(&EditorID);
        return fields;
    }

    void SetFlag(RecordFlags flag) {m_flags |= (uint16_t)flag;}
    void ClearFlag(RecordFlags flag) {m_flags &= ~(uint16_t)flag;}
    bool HasFlag(RecordFlags flag) {return (m_flags & (uint16_t)flag) == (uint16_t)flag;}

    // Marks Record Dirty, meaning that the Record has changed and should be saved again
    void Dirty();
    void BeginUnload() {m_flags |= (uint16_t)RecordFlags::Unload;}
};

template<typename T>
concept RecordClass = std::is_base_of<Record, T>::value;