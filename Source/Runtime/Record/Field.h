#pragma once

#include <cstdint>

#include "FieldBase.h"

/*
 * Field allows seemless serialization of persistant data in records
 */
template <FieldValueClass FieldValue>
class Field : public FieldBase
{
    uint32_t m_id;

    FieldValue::DecayType m_value{};
    bool m_isDirty = false;
public:
    Field(uint32_t id) : m_id(id) {}
    Field(uint32_t id, FieldValue::DecayType defaultValue) : m_id(id), m_value(defaultValue), m_isDirty(true) {}

    Field<FieldValue>& operator=(FieldValue::DecayType&& value) {m_value = std::move(value); m_isDirty = true; return *this;}
    Field<FieldValue>& operator=(FieldValue::DecayType& value) {m_value = value; m_isDirty = true; return *this;}

    operator const typename FieldValue::DecayType&() const {return m_value;}

    inline bool IsDirty() const override {return m_isDirty;}

    void Serialize() override {
        std::vector<uint8_t> temp;
        FieldValue::Serialize(m_value, temp);
        m_isDirty = false;
    }

    void Deserialize() override {
        std::span<const uint8_t> temp;
        FieldValue::Deserialize(temp, m_value);
        m_isDirty = false;
    }
};