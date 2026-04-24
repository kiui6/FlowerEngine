#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <array>
#include <concepts>

#include <Utility/Record.h>

#include "Field.h"
#include "RecordRefCount.h"

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
 * Each Record can contain multiple Children, as well as own Parent Record  
 * 
 * Record ID 0 is reserved for None; 
 */
class Record : public RecordRefCount
{
protected:
    uint16_t m_flags = 0;
    RecordID m_id = INVALID_RECORD;
    std::string m_name = "";
    uint32_t m_type;
    
    std::vector<Record*> m_children;
    Record* m_parent = nullptr;

    Field<StringField> f_edid = {FIELDID(EDID), "Untitled"};
public:
    Record() {}
    Record(uint32_t id) {m_id = id;}
    virtual ~Record(){}

    static uint32_t StaticType() {return MakeRecordType("UNKN");}
    uint32_t& GetType() {return m_type;}

    void Reparent(Record* newParent);
    void Rename(const std::string& newName);
    // TODO: Maybe make this private? It can cause big issues
    void SetID(RecordID newID) {m_id = newID;}

    inline RecordID GetID() const {return m_id;}
    inline const std::string& GetName() const {return m_name;}

    void SetFlag(RecordFlags flag) {m_flags |= (uint16_t)flag;}
    void ClearFlag(RecordFlags flag) {m_flags &= ~(uint16_t)flag;}
    bool HasFlag(RecordFlags flag) {return (m_flags & (uint16_t)flag) == (uint16_t)flag;}

    // Marks Record Dirty, meaning that the Record has changed and should be saved again
    void Dirty();
    void BeginUnload() {m_flags |= (uint16_t)RecordFlags::Unload;}

    template <typename ChildRecordT = Record>
    inline std::vector<ChildRecordT*> GetChildren() const {
        std::vector<ChildRecordT*> typedChildrenArray {};
        for(Record* child : m_children)
        {
            if(ChildRecordT* typedChild = dynamic_cast<ChildRecordT*>(child))
            {
                typedChildrenArray.push_back(typedChild);
            }
        }
        return m_children;
    }
    
    template <typename ParentRecordT = Record>
    ParentRecordT* GetParent() 
    {
        // If parent doesn't match the template type, attempt to find next parent
        if(dynamic_cast<ParentRecordT*>(m_parent) == nullptr && m_parent != nullptr) {
            return m_parent->GetParent<ParentRecordT>();
        } else {
            return dynamic_cast<ParentRecordT*>(m_parent);
        }
    }
};

template<typename T>
concept RecordClass = std::is_base_of<Record, T>::value;