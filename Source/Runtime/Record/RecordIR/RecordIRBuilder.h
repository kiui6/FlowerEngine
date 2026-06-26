#pragma once

#include "RecordObject.h"

#include <Utility/Memory/Arena.h>

class RecordIRBuilder
{
    GrowingArena<16*1024> m_nodeArena;
    RecordObject m_recordObject;

    class NodeWrapper {
        RecordIRBuilder* m_builder;
        FieldNode* m_node;
    public:
        NodeWrapper(RecordIRBuilder* builder, FieldNode* node) : m_builder(builder), m_node(node) {}

        inline NodeWrapper& SetOp(FieldNodeOp op) {m_node->op = op; return *this;}
        inline NodeWrapper& SetSize(uint16_t size) {m_node->size = size; return *this;}
        inline NodeWrapper& SetElementNumber(uint16_t elementNumber) {m_node->elementNumber = elementNumber; return *this;}

        inline NodeWrapper& SetBool(bool value) {m_node->data.boolValue = value; return *this;}
        inline NodeWrapper& SetUnsigned(uint64_t value) {m_node->data.unsignedValue = value; return *this;}
        inline NodeWrapper& SetInt(int64_t value) {m_node->data.integerValue = value; return *this;}
        inline NodeWrapper& SetString(const char* value) {m_node->data.stringValue = value; return *this;}
        inline NodeWrapper& SetFloat(float value) {m_node->data.floatValue = value; return *this;}
        inline NodeWrapper& SetDouble(double value) {m_node->data.doubleValue = value; return *this;}
        inline NodeWrapper& MakeChild(FieldNodeType type) {
            FieldNode* childNode = m_builder->m_nodeArena.AllocateObject<FieldNode>();
            childNode->type = type;
            m_node->data.children = childNode;
            m_node = childNode;
            return *this;
        }

        inline NodeWrapper& MakeNext(FieldNodeType type) {
            m_node = m_builder->m_nodeArena.AllocateObject<FieldNode>();
            m_node->type = type;
            return *this;
        }
    };
public:

    inline NodeWrapper CreateField(ID32 id, FieldNodeType type) {
        FieldNode* node = m_nodeArena.AllocateObject<FieldNode>();
        m_recordObject.PushField(id, node);
        return NodeWrapper(this, node);
    }

    inline void SetID(uint64_t id) {/*TODO*/}

    // Can be used to move
    inline RecordObject& GetRecordObject() { return m_recordObject;}

    inline void Reset() {
        m_nodeArena.Reset();
        m_recordObject.Reset();
    }
};