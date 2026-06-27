#pragma once

#include "FieldNode.h"
#include <cstdint>

#include <Utility/Containers/FlatHashMap.h>
#include <Utility/ID.h>
#include <Utility/Memory/Arena.h>

class RecordObject
{
    GrowingArena<8*1024> m_nodeArena;
    FlatHashMap<ID32, FieldNode*> m_fields;
    uint64_t m_id = 0;
    uint32_t m_type = 0;
    bool m_deleted = false;
    bool m_delta = false;
public:
    class NodeWrapper {
        RecordObject* m_object;
        FieldNode* m_node;
    public:
        NodeWrapper(RecordObject* object, FieldNode* node) : m_object(object), m_node(node) {}

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
            FieldNode* childNode = m_object->m_nodeArena.AllocateObject<FieldNode>();
            childNode->type = type;
            m_node->data.children = childNode;
            m_node = childNode;
            return *this;
        }

        inline NodeWrapper& MakeNext(FieldNodeType type) {
            m_node = m_object->m_nodeArena.AllocateObject<FieldNode>();
            m_node->type = type;
            return *this;
        }
    };

    inline NodeWrapper CreateField(ID32 id, FieldNodeType type) {
        FieldNode* node = m_nodeArena.AllocateObject<FieldNode>();
        node->type = type;
        m_fields.Emplace(id, node);
        return NodeWrapper(this, node);
    }

    void SetID(uint64_t id) {m_id = id;}
    uint64_t GetID() const {return m_id;}
    void SetType(uint32_t type) {m_type = type;}
    uint32_t GetType() const {return m_type;}
    
    void SetDeleted(bool deleted) {m_deleted = deleted;}
    bool GetDeleted() {return m_deleted;}

    inline bool IsEmpty() const {return m_fields.Empty() && m_nodeArena.IsEmpty();}

    inline void Reset() {
        m_id = 0;
        m_deleted = false;
        m_delta = false;
        m_fields.Clear();
        m_nodeArena.Reset();
    }
};