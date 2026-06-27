#pragma once

#include "FieldNode.h"
#include <cstdint>

#include <Utility/Containers/FlatHashMap.h>
#include <Utility/ID.h>
#include <Utility/Memory/Arena.h>

class RecordIRAllocator {
    GrowingArena<8*1024> m_nodeArena{};
public:
    FieldNode* Allocate() {return m_nodeArena.AllocateObject<FieldNode>();}
    void Reset() {m_nodeArena.Reset();}
    bool Empty() const {return m_nodeArena.IsEmpty();}
};

/*
 * Contains a snapshot of fields from a specific plugin for a specific record
 */
class RecordFieldObject {
    RecordIRAllocator& m_alloc;
    FlatHashMap<ID32, FieldNode*> m_fields{};
    bool m_delta = false;
public:
    RecordFieldObject(RecordIRAllocator& alloc) : m_alloc(alloc) {}

    class NodeWrapper {
        RecordIRAllocator& m_alloc;
        FieldNode* m_node;
    public:
        NodeWrapper(RecordIRAllocator& alloc, FieldNode* node) : m_alloc(alloc), m_node(node) {}

        inline FieldNode* GetNode() const {return m_node;}

        inline NodeWrapper& SetOp(FieldNodeOp op) {m_node->op = op; return *this;}
        inline NodeWrapper& SetSize(uint16_t size) {m_node->size = size; return *this;}
        inline NodeWrapper& SetSourcePoition(uint16_t pos) {m_node->sourcePosition = pos; return *this;}
        inline NodeWrapper& SetTargetPoition(uint16_t pos) {m_node->targetPosition = pos; return *this;}

        inline NodeWrapper& SetBool(bool value) {m_node->data.boolValue = value; return *this;}
        inline NodeWrapper& SetUnsigned(uint64_t value) {m_node->data.unsignedValue = value; return *this;}
        inline NodeWrapper& SetInt(int64_t value) {m_node->data.integerValue = value; return *this;}
        inline NodeWrapper& SetString(std::string_view value) {m_node->data.stringValue = value.data(); return *this;}
        inline NodeWrapper& SetFloat(float value) {m_node->data.floatValue = value; return *this;}
        inline NodeWrapper& SetDouble(double value) {m_node->data.doubleValue = value; return *this;}
        inline NodeWrapper& MakeChild(FieldNodeType type) {
            FieldNode* childNode = m_alloc.Allocate();
            childNode->type = type;
            m_node->data.children = childNode;
            m_node = childNode;
            return *this;
        }

        inline NodeWrapper& MakeNext(FieldNodeType type) {
            m_node = m_alloc.Allocate();
            m_node->type = type;
            return *this;
        }
    };

    inline NodeWrapper CreateField(ID32 id, FieldNodeType type) {
        FieldNode* node = m_alloc.Allocate();
        node->type = type;
        m_fields.Emplace(id, node);
        return NodeWrapper(m_alloc, node);
    }
};

class RecordObject
{
    RecordIRAllocator m_alloc{};
    std::vector<RecordFieldObject> m_fieldObjects{};
    uint64_t m_id = 0;
    uint32_t m_type = 0;
    bool m_deleted = false;
    RecordFieldObject* m_finalFO = nullptr;

    friend class RecordFieldObject;
public:
    void SetID(uint64_t id) {m_id = id;}
    uint64_t GetID() const {return m_id;}
    void SetType(uint32_t type) {m_type = type;}
    uint32_t GetType() const {return m_type;}
    
    void SetDeleted(bool deleted) {m_deleted = deleted;}
    bool GetDeleted() const {return m_deleted;}

    RecordFieldObject& CreateFieldObject() {return m_fieldObjects.emplace_back(RecordFieldObject(m_alloc));}

    void SetFinal(size_t finalID) const {}
    RecordFieldObject* GetFinal() const {return m_finalFO;}

    inline bool IsEmpty() const {return m_fieldObjects.empty() && m_alloc.Empty();}

    inline void Reset() {
        m_id = 0;
        m_type = 0;
        m_deleted = false;
        m_finalFO = nullptr;
        m_alloc.Reset();
    }
};