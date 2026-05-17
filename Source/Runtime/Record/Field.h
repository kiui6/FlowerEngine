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
public:
    Field(uint32_t id) : m_id(id) {}
    Field(uint32_t id, FieldValue::DecayType defaultValue) : m_id(id), m_value(defaultValue) {}

    FieldValue::DecayType& Get() const { return m_value; }
    void Set(FieldValue::DecayType&& value) { m_value = value; }

    Field<FieldValue>& operator=(FieldValue::DecayType&& value) {m_value = std::move(value); return *this;}
    Field<FieldValue>& operator=(FieldValue::DecayType& value) {m_value = value; return *this;}

    operator typename FieldValue::DecayType&() {return m_value;}

    void Serialize() override {
        std::vector<uint8_t> temp;
        FieldValue::Serialize(m_value, temp);
    }

    void Deserialize() override {
        std::span<const uint8_t> temp;
        FieldValue::Deserialize(temp, m_value);
    }
};