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

    FieldValue::DecayType m_cached;
public:
    Field(uint32_t id) : m_id(id), m_cached(FieldValue::DefaultConstructor()) {}
    Field(uint32_t id, FieldValue::DecayType defaultValue) : m_id(id), m_cached(defaultValue) {}

    FieldValue::DecayType& Get() const { return m_cached; }
    void Set(FieldValue::DecayType&& value) { m_cached = value; }

    Field<FieldValue>& operator=(FieldValue::DecayType&& value) {m_cached = std::move(value); return *this;}
    Field<FieldValue>& operator=(FieldValue::DecayType& value) {m_cached = value; return *this;}

    operator typename FieldValue::DecayType&() {return m_cached;}

    void Serialize() override {
        std::vector<uint8_t> temp;
        FieldValue::Serialize(m_cached, temp);
    }

    void Deserialize() override {
        std::span<const uint8_t> temp;
        FieldValue::Deserialize(temp, m_cached);
    }
};