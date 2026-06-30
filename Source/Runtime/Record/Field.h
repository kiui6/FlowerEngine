#pragma once

#include <cstdint>

#include "FieldBase.h"

/*
 * Field allows seemless serialization of persistant data in records
 */
template <FieldValueClass FieldValue>
class Field : public FieldBase
{
    ID32 m_id;

    FieldValue::DecayType m_value{};
    bool m_isDirty = false;
public:
    Field(ID32 id) : m_id(id) {}
    Field(ID32 id, FieldValue::DecayType defaultValue) : m_id(id), m_value(defaultValue), m_isDirty(true) {}

    Field<FieldValue>& operator=(FieldValue::DecayType&& value) {m_value = std::move(value); m_isDirty = true; return *this;}
    Field<FieldValue>& operator=(FieldValue::DecayType& value) {m_value = value; m_isDirty = true; return *this;}

    operator const typename FieldValue::DecayType&() const {return m_value;}

    inline ID32 GetID() const override {return m_id;}
    
    typename FieldValue::DecayType& Get() {return m_value;}
    const typename FieldValue::DecayType& Get() const {return m_value;}

    inline bool IsDirty() const override {return m_isDirty;}

    void Serialize(RecordObject& builder) override {
        //FieldValue::Serialize(m_value,);
        m_isDirty = false;
    }

    void Deserialize(const FieldNode& node) override {
        FieldValue::Deserialize(node, m_value);
        m_isDirty = false;
    }
};